// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <set>
#include <unordered_map>
#include <vector>
#include <list>

#include "spdlog/spdlog.h"

#include "binom_client.h"
#include "http.h"

// insert period and campaign_id into tracker url as query parameters
// default value for group1 is 2 because in
// tracker 2 means filter by paths (default way)
inline std::string _build_request_url(
                        const std::string base_url,
                        const std::string period,
                        const std::string campaign_id,
                        const std::string group_1 = "2") {
    return base_url + period + "&camp_id=" + campaign_id + "&group1=" + group_1;
}

namespace binom {
    std::vector<std::string> get_field_values(const std::string field_name,
                                    const std::string data) {
        std::vector<std::string> result;

        std::string pattern = "\"" + field_name + "\":\"";
        if (data.find(pattern) == std::string::npos) {
            throw http::IncorrectResponse();
        }

        size_t start_pos = 0, end_pos;

        while (data.find(pattern, start_pos) != std::string::npos) {
            start_pos = data.find(pattern, start_pos) + pattern.length();
            size_t end_pos = data.find("\"", start_pos);
            auto str = data.substr(start_pos, end_pos - start_pos);

            result.emplace_back(str);
        }

        return result;
    }

// private namespace
namespace {
    std::set<std::string> get_zones_names(const std::string zones_info) {
        std::vector<std::string> result;

        try {
            result = binom::get_field_values("name", zones_info);
            return std::set<std::string>(result.begin(), result.end());
        }
        catch (http::IncorrectResponse) {
            spdlog::get("actions_logger")->error("Empty zones info - "
                                                 "can't extract names "
                                                 "(incorrect response).");
        }
        catch (http::RequestError) {
            spdlog::get("actions_logger")->error("Request error while tryi"
                                                    "ng to get zones' names.");
        }

        return {};
    }

    std::unordered_map<std::string, double> extract_zone_info(
                                            const std::string zone,
                                            const std::string zones_info) {
        size_t start = zones_info.find(zone), end = zones_info.find("}");
        std::unordered_map<std::string, double> result;

        auto zone_info = zones_info.substr(start, end - start);

        double cost = stod((*(binom::get_field_values("cost",
                                                    zone_info).begin())));
        double revenue = stod((*(binom::get_field_values("revenue",
                                                    zone_info).begin())));
        double clicks = stod((*(binom::get_field_values("clicks",
                                                    zone_info).begin())));
        int leads = stoi((*(binom::get_field_values("leads",
                                                    zone_info).begin())));

        return binom::calculate_statistics(cost, revenue, clicks, leads);
    }

    std::unordered_map<std::string, double> calculate_statistics(
                                                    const double cost,
                                                    const double revenue,
                                                    const int clicks,
                                                    const int leads) {
        return {{"cost", cost},
                {"revenue", revenue},
                {"clicks", clicks},
                {"leads", leads},
                {"profit", PROFIT(revenue, cost)},
                {"ROI", ROI(revenue, cost)},
                {"CR", CR(leads, clicks)},
                {"EPC", EPC(revenue, clicks)},
                {"CPC", CPC(cost, clicks)},
                {"CPA", CPA(cost, leads)}};
    }
}  // namespace

    std::unordered_map<std::string, double> get_campaign_info(
                                            const size_t campaign_tracker_id,
                                            const size_t period) {
        std::list<std::string> headers = {"Content-Type: application/json",
                                          "Accept: application/json"};

        auto url = _build_request_url(binom::tracker_requests_url,
                                      std::to_string(period),
                                      std::to_string(campaign_tracker_id));

        float cost = 0., revenue = 0., clicks = 0.;
        int leads = 0;

        spdlog::get("actions_logger")->info("Perform request: " + url);

        auto campaign_info = http::make_request(headers, std::string(), 
                                                url, "GET");

        if (campaign_info.size() == 0) {
            spdlog::get("actions_logger")->error("Error while trying to make"
                                                 " request (or empty result)");
            return {};
        }

        // empty campaign info - returning default
        if (campaign_info == NO_CLICKS) {
            return binom::calculate_statistics(0., 0., 0, 0);
        }

        try {
            auto costs_by_paths = binom::get_field_values("cost",
                                                          campaign_info);
            auto revenues_by_paths = binom::get_field_values("revenue",
                                                             campaign_info);
            auto clicks_by_paths = binom::get_field_values("clicks",
                                                           campaign_info);
            auto leads_by_paths = binom::get_field_values("leads",
                                                          campaign_info);

            for (auto cost_path : costs_by_paths) {
                cost += stof(cost_path);
            }

            for (auto revenue_path : revenues_by_paths) {
                revenue += stof(revenue_path);
            }

            for (auto clicks_path : clicks_by_paths) {
                clicks += stof(clicks_path);
            }

            for (auto leads_path : leads_by_paths) {
                leads += stoi(leads_path);
            }

            return binom::calculate_statistics(cost, revenue, clicks, leads);
        }
        catch (const std::invalid_argument& exc) {
            spdlog::get("actions_logger")->error(exc.what());
        }
        catch (http::IncorrectResponse) {
            spdlog::get("actions_logger")->error(
                "Incorrect response while trying to get info about campaign: "
                + std::to_string(campaign_tracker_id));
        }
        catch (http::RequestError) {
            spdlog::get("actions_logger")->error(
                "Request error while trying to get info about campaign: "
                + std::to_string(campaign_tracker_id));
        }

        spdlog::get("actions_logger")->error("Incorrect response from tracker: "
                                            + campaign_info);

        return {};
    }

    zones_data get_zones_info(const size_t campaign_tracker_id,
                              const size_t period,
                              const std::string zones_param_number,
                              const std::set<std::string>& ignored_zones) {
        std::list<std::string> headers = {"Content-Type: application/json",
                                          "Accept: application/json"};
        auto url = _build_request_url(binom::tracker_requests_url,
                                      std::to_string(period),
                                      std::to_string(campaign_tracker_id),
                                      zones_param_number) + "&val_page=All";
        std::string zones_info;

        spdlog::get("actions_logger")->info("Perform request: " + url);

        try {
            zones_info = http::make_request(headers, std::string(), url, "GET");
        }
        catch(http::RequestError) {
            spdlog::get("actions_logger")->error(
                "RequestError while trying to get zones info from tracker."
                " Campaign id: " + std::to_string(campaign_tracker_id));
            return {};
        }

        if (zones_info.size() == 0) {
            spdlog::get("actions_logger")->error(
                "Error or empty result while trying to get zones info "
                "from tracker. Campaign id: "
                + std::to_string(campaign_tracker_id));
            return {};
        }

        // empty campaign info - returning default value
        if (zones_info == NO_CLICKS) {
            return {{NO_CLICKS, {}}};
        }

        auto zones_names = binom::get_zones_names(zones_info);
        zones_data result;

        std::set<std::string> final_zones_names;
        set_difference(zones_names.begin(), zones_names.end(),
                       ignored_zones.begin(), ignored_zones.end(),
                       inserter(final_zones_names, final_zones_names.end()));

        for (auto& zone : final_zones_names) {
            try {
                result.push_back({zone,
                                  binom::extract_zone_info(zone, zones_info)});
            }
            catch (http::IncorrectResponse) {
                spdlog::get("actions_logger")->error(
                    "Can't get zone info: "+ zone + ": incorrect response"
                    " exception. Campaign id: " +
                    std::to_string(campaign_tracker_id));
            }
            catch (http::RequestError) {
                spdlog::get("actions_logger")->error(
                    "Can't get zone info: " + zone + "request error. "
                    "Campaign id: " + std::to_string(campaign_tracker_id));
            }
            catch (const std::invalid_argument& exc) {
                spdlog::get("actions_logger")->error(
                    "Invalid argument occurred while trying to extract zone"
                    + zone + " info: " + std::string(exc.what())
                    + ". Campaign id: " + std::to_string(campaign_tracker_id));
            }
        }

        return result;
    }
}  // namespace binom
