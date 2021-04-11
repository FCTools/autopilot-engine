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
#include <iostream>

#include "spdlog/spdlog.h"

#include "tracker_controllers/binom_client.h"
#include "helpers/http.h"

// insert period and campaign_id into tracker url as query parameters
// default value for group1 is 2 because in
// tracker 2 means filter by paths (default way)
inline std::string _build_request_url(const std::string base_url, const std::string period,
                                      const std::string campaign_id, const std::string group_1 = "2")
{
    return base_url + period + "&camp_id=" + campaign_id + "&group1=" + group_1;
}

namespace binom
{
    std::vector<std::string> get_field_values(const std::string field_name, const std::string &data)
    {
        std::vector<std::string> result;

        std::string search_pattern = "\"" + field_name + "\":\"";
        size_t pattern_len = search_pattern.length();

        if (data.find(search_pattern) == std::string::npos)
        {
            throw http::IncorrectResponse();
        }

        size_t start_pos = 0, end_pos;

        while (data.find(search_pattern, start_pos) != std::string::npos)
        {
            start_pos = data.find(search_pattern, start_pos) + pattern_len;
            end_pos = data.find("\"", start_pos);
            auto target_str = data.substr(start_pos, end_pos - start_pos);

            result.emplace_back(target_str);
        }

        return result;
    }

    zones_data extract_zones_info(std::string &zones_info, const std::set<std::string> &ignored_zones)
    {
        zones_data result;

        std::string search_pattern = "\"name\":\"";
        size_t start_pos = 0, end_pos, pattern_len = search_pattern.length();

        if (zones_info.find(search_pattern) == std::string::npos)
        {
            throw http::RequestError();
        }

        while (zones_info.find(search_pattern, start_pos) != std::string::npos)
        {
            start_pos = zones_info.find(search_pattern, start_pos) + pattern_len;
            end_pos = zones_info.find("\"", start_pos);
            auto name = zones_info.substr(start_pos, end_pos - start_pos);

            if (ignored_zones.find(name) != ignored_zones.end())
            {
                continue;
            }

            auto end = zones_info.find("}");
            auto zone_info = zones_info.substr(end_pos, end - end_pos);

            double cost = stod((*(binom::get_field_values("cost", zone_info).begin())));
            double revenue = stod((*(binom::get_field_values("revenue", zone_info).begin())));
            double clicks = stod((*(binom::get_field_values("clicks", zone_info).begin())));
            int leads = stoi((*(binom::get_field_values("leads", zone_info).begin())));

            auto statistics = binom::calculate_statistics(cost, revenue, clicks, leads);

            result.push_back({name, statistics});
        }

        return result;
    }

    // private namespace
    namespace
    {
        std::unordered_map<std::string, double> calculate_statistics(const double cost, const double revenue,
                                                                     const int clicks, const int leads)
        {
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
    } // namespace

    std::unordered_map<std::string, double> get_campaign_info(
                                                            const std::string campaign_tracker_id,
                                                            const std::string tracker_requests_url,
                                                            const std::string tracker_api_key,
                                                            const size_t period)
    {
        std::list<std::string> headers = {"Content-Type: application/json", "Accept: application/json"};
        std::unordered_map<std::string, std::string> params = {{"page", "Stats"},
                                                               {"group2", "1"},
                                                               {"group3", "1"},
                                                               {"api_key", tracker_api_key},
                                                               {"date", std::to_string(period)},
                                                               {"camp_id", campaign_tracker_id},
                                                               {"group1", "2"}};

        auto request_url = http::build_url(tracker_requests_url, params);
                    

        // TODO: remove hardcoded value: 5 tries
        const size_t default_tries = 5;
        size_t tries = default_tries;

        float cost = 0., revenue = 0., clicks = 0.;
        int leads = 0;
        std::string campaign_info;

        while (tries != 0)
        {
            spdlog::get("actions_logger")->info("Perform request: " + request_url);

            campaign_info = http::make_request(headers, std::string(), request_url, "GET");

            if (campaign_info.size() == 0)
            {
                spdlog::get("actions_logger")->error("Error while trying to make request (or empty result)");

                tries--;

                // TODO: remove hardcoded value: 5 seconds
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            // empty campaign info - returning default
            if (campaign_info == NO_CLICKS)
            {
                return binom::calculate_statistics(0., 0., 0, 0);
            }

            try
            {
                auto costs_by_paths = binom::get_field_values("cost", campaign_info);
                auto revenues_by_paths = binom::get_field_values("revenue", campaign_info);
                auto clicks_by_paths = binom::get_field_values("clicks", campaign_info);
                auto leads_by_paths = binom::get_field_values("leads", campaign_info);

                for (auto cost_path : costs_by_paths)
                {
                    cost += stof(cost_path);
                }

                for (auto revenue_path : revenues_by_paths)
                {
                    revenue += stof(revenue_path);
                }

                for (auto clicks_path : clicks_by_paths)
                {
                    clicks += stof(clicks_path);
                }

                for (auto leads_path : leads_by_paths)
                {
                    leads += stoi(leads_path);
                }

                return binom::calculate_statistics(cost, revenue, clicks, leads);
            }
            catch (const std::invalid_argument &exc)
            {
                spdlog::get("actions_logger")->error(exc.what());

                tries--;

                // TODO: remove hardcoded value: 5 seconds
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            catch (http::IncorrectResponse)
            {
                spdlog::get("actions_logger")->error(
                    "Incorrect response while trying to get info about campaign: " + campaign_tracker_id);
                
                tries--;

                // TODO: remove hardcoded value: 5 seconds
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            catch (http::RequestError)
            {
                spdlog::get("actions_logger")->error(
                    "Request error while trying to get info about campaign: " + campaign_tracker_id);

                tries--;

                // TODO: remove hardcoded value: 5 seconds
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        }

        spdlog::get("actions_logger")->error("Incorrect response from tracker: " + campaign_info);

        return {};
    }

    zones_data get_zones_info(const std::string campaign_tracker_id,
                              const std::string tracker_requests_url,
                              const std::string tracker_api_key,
                              const size_t period,
                              const std::string zones_param_number,
                              const std::set<std::string> &ignored_zones)
    {
        std::list<std::string> headers = {"Content-Type: application/json", "Accept: application/json"};
        std::unordered_map<std::string, std::string> params = {{"page", "Stats"},
                                                               {"group2", "1"},
                                                               {"group3", "1"},
                                                               {"api_key", tracker_api_key},
                                                               {"date", std::to_string(period)},
                                                               {"camp_id", campaign_tracker_id},
                                                               {"group1", zones_param_number},
                                                               {"val_page", std::to_string(ZONES_PER_PAGE)}};

        auto request_url = http::build_url(tracker_requests_url, params);
        
        std::string tmp_zones_info;
        zones_data zones_info, zones_page;

        size_t page_number = 1;

        // TODO: remove hardcoded value: 5 tries
        const size_t default_tries = 5;
        size_t tries = default_tries;

        while (tries != 0)
        {
            try
            {
                while (true)
                {
                    tmp_zones_info = http::make_request(headers, std::string(),
                        request_url + "&num_page=" + std::to_string(page_number), "GET");
                    
                    if (tmp_zones_info == "null") {
                        break;
                    }

                    spdlog::get("actions_logger")->info("Perform request: " + request_url
                                                        + "&num_page=" + std::to_string(page_number));

                    zones_page = binom::extract_zones_info(tmp_zones_info, ignored_zones);
                    zones_info.insert(zones_info.end(), zones_page.begin(), zones_page.end());

                    if (zones_page.size() < ZONES_PER_PAGE)
                    {
                        break;
                    }
                    zones_page.clear();
                    page_number++;
                }

                if (tmp_zones_info == NO_CLICKS)
                {
                    return {{NO_CLICKS, {}}};
                }

                return zones_info;
            } 
            catch (http::RequestError)
            {
                spdlog::get("actions_logger")->error("RequestError while trying to get zones info from tracker."
                                                     " Campaign id: " +
                                                     campaign_tracker_id);
            }

            tries--;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        if (tmp_zones_info.size() == 0)
        {
            spdlog::get("actions_logger")->error("Error or empty result while trying to get zones info "
                                                 "from tracker. Campaign id: " +
                                                 campaign_tracker_id);
            return {};
        }

        return zones_info;
    }
} // namespace binom
