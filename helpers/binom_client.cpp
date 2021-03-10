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

using namespace std;

// insert period and campaign_id into tracker url as query parameters
// default value for group1 is 2 because in
// tracker 2 means filter by paths (default way)
inline string _build_request_url(
                        const string base_url, const string period, 
                        const string campaign_id, const string group_1 = "2") {
    return base_url + period + "&camp_id=" + campaign_id + "&group1=" + group_1;
}

namespace binom {
    vector<string> get_field_values(const string field_name, 
                                    const string data) {
        vector<string> result;

        string pattern = "\"" + field_name + "\":\"";
        if (data.find(pattern) == string::npos) {
            throw http::IncorrectResponse();
        }

        size_t start_pos = 0, end_pos;

        while (data.find(pattern, start_pos) != string::npos) {
            start_pos = data.find(pattern, start_pos) + pattern.length();
            size_t end_pos = data.find("\"", start_pos);
            string str = data.substr(start_pos, end_pos - start_pos);

            result.emplace_back(str);
        }

        return result;
    }

    // private namespace members are here
    namespace {
        set<string> get_zones_names(const string zones_info) {
            vector<string> result;

            try {
                result = binom::get_field_values("name", zones_info);
                return set<string>(result.begin(), result.end());
            }
            catch (http::IncorrectResponse) {
                spdlog::get("actions_logger")->error("Empty zones info - can't extract names (incorrect response).");
            }
            catch (http::RequestError) {
                spdlog::get("actions_logger")->error("Request error while trying to get zones' names.");
            }

            return {};
        }

        unordered_map<string, double> extract_zone_info(const string zone, const string zones_info) {
            size_t start = zones_info.find(zone), end = zones_info.find("}");
            unordered_map<string, double> result;

            string zone_info = zones_info.substr(start, end - start);

            double cost = stod((*(binom::get_field_values("cost", zone_info).begin())));
            double revenue = stod((*(binom::get_field_values("revenue", zone_info).begin())));
            double clicks = stod((*(binom::get_field_values("clicks", zone_info).begin())));
            int leads = stoi((*(binom::get_field_values("leads", zone_info).begin())));

            return binom::calculate_statistics(cost, revenue, clicks, leads);
        }


        unordered_map<string, double> calculate_statistics(const double cost, const double revenue, 
                                                           const int clicks, const int leads) {
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
    }

    unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const size_t period) {
        list<string> headers = {"Content-Type: application/json", "Accept: application/json"};
        const string url = _build_request_url(binom::tracker_requests_url, to_string(period), to_string(campaign_tracker_id));

        float cost = 0., revenue = 0., clicks = 0.;
        int leads = 0;

        spdlog::get("actions_logger")->info("Perform request: " + url);

        string campaign_info = http::make_request(headers, string(), url, "GET");

        if (campaign_info.size() == 0) {
            spdlog::get("actions_logger")->error("Error while trying to make request (or empty result)");
            return {};
        }

        try {
            vector<string> costs_by_paths = binom::get_field_values("cost", campaign_info);
            vector<string> revenues_by_paths = binom::get_field_values("revenue", campaign_info);
            vector<string> clicks_by_paths = binom::get_field_values("clicks", campaign_info);
            vector<string> leads_by_paths = binom::get_field_values("leads", campaign_info);

            for (string cost_path: costs_by_paths) {
                cost += stof(cost_path);
            }
            
            for (string revenue_path: revenues_by_paths) {
                revenue += stof(revenue_path);
            }

            for (string clicks_path: clicks_by_paths) {
                clicks += stof(clicks_path);
            }

            for (string leads_path: leads_by_paths) {
                leads += stoi(leads_path);
            }

            return binom::calculate_statistics(cost, revenue, clicks, leads);
        }
        catch (const invalid_argument& exc) {
            spdlog::get("actions_logger")->error(exc.what());
        }
        catch (http::IncorrectResponse) {
            spdlog::get("actions_logger")->error("Incorrect response while trying to get info about campaign: " + to_string(campaign_tracker_id));
        }
        catch (http::RequestError) {
            spdlog::get("actions_logger")->error("Request error while trying to get info about campaign: " + to_string(campaign_tracker_id));
        }

        spdlog::get("actions_logger")->error("Incorrect response from tracker: " + campaign_info);

        return {};
    }

    zones_data get_zones_info(const size_t campaign_tracker_id, const size_t period, 
                                     const string zones_param_number, const set<string>& ignored_zones) {
    list<string> headers = {"Content-Type: application/json", "Accept: application/json"};
        const string url = _build_request_url(binom::tracker_requests_url, to_string(period), to_string(campaign_tracker_id),
                                            zones_param_number) + "&val_page=All";
        string zones_info;

        spdlog::get("actions_logger")->info("Perform request: " + url);

        try {
            zones_info = http::make_request(headers, string(), url, "GET");
        }
        catch(http::RequestError) {
            spdlog::get("actions_logger")->error("RequestError while trying to get zones info from tracker. Campaign id: " + to_string(campaign_tracker_id));
            return {};
        }

        if (zones_info.size() == 0) {
            spdlog::get("actions_logger")->error("Error or empty result while trying to get zones info from tracker. Campaign id: " + to_string(campaign_tracker_id));
            return {};
        }

        set<string> zones_names = binom::get_zones_names(zones_info);
        zones_data result;

        set<string> final_zones_names;
        set_difference(zones_names.begin(), zones_names.end(), ignored_zones.begin(), ignored_zones.end(), 
                    inserter(final_zones_names, final_zones_names.end()));

        for (auto& zone: final_zones_names) {
            try {
                result.push_back({zone, binom::extract_zone_info(zone, zones_info)});
            }
            catch (http::IncorrectResponse) {
                spdlog::get("actions_logger")->error("Can't get zone info: " + zone + ": incorrect response exception. Campaign id: " + to_string(campaign_tracker_id));
            }
            catch (http::RequestError) {
                spdlog::get("actions_logger")->error("Can't get zone info: " + zone + "request error. Campaign id: " + to_string(campaign_tracker_id));
            }
            catch (const invalid_argument& exc) {
                spdlog::get("actions_logger")->error("Invalid argument occurred while trying to extract zone" + zone + " info: "
                                                    + string(exc.what()) + ". Campaign id: " + to_string(campaign_tracker_id));
            }
        }

        return result;
    }
}
