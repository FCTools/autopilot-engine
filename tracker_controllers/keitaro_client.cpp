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

#include "tracker_controllers/keitaro_client.h"
#include "helpers/http.h"
#include "helpers/time_.h"

namespace keitaro
{
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

        std::string dump_to_string(std::unordered_map<std::string, std::string> &data)
        {
            std::string result = "{";

            for (auto item : data)
            {
                result += "\"" + item.first + "\":" + item.second + ",";
            }

            result = result.substr(0, result.size() - 1) + "}";

            return result;
        }

        // tested
        std::pair<std::string, std::string> get_range_this_month()
        {
            return {get_past_time((stoi(get_day(0)) - 1) * 24 * 60 * 60), get_now()};
        }

        // tested
        std::pair<std::string, std::string> get_range_last_month()
        {
            std::string last_month_last_day = get_past_time(stoi(get_day(0)) * 24 * 60 * 60);
            auto last_month_last_day_int = stoi(get_day(stoi(get_day(0)) * 24 * 60 * 60));
            
            std::string last_month_first_day = get_past_time((stoi(get_day(0)) - 1) * 24 * 60 * 60 +
                                                            last_month_last_day_int * 24 * 60 * 60);
            return {last_month_first_day, last_month_last_day};
        }

        // tested
        std::pair<std::string, std::string> get_range_last_n_days(std::size_t days_number)
        {
            return {get_past_time(days_number * 24 * 60 * 60), get_now()};
        }

        // tested
        std::pair<std::string, std::string> get_range_this_year()
        {
            int days = 0;

            while (stoi(get_month((stoi(get_day(0)) + days) * 24 * 60 * 60)) != 12)
            {
                auto last_month_last_day_int = stoi(get_day((stoi(get_day(0)) + days) * 24 * 60 * 60));
                days += last_month_last_day_int;
            }

            days += stoi(get_day(0)) - 1;

            auto first_day = get_past_time(days * 24 * 60 * 60);

            return {first_day, get_now()};
        }

        std::pair<std::string, std::string> get_range_this_week()
        {
            return {"", ""};
        }

        std::string calculate_range(std::size_t period)
        {
            std::unordered_map<std::string, std::string> result;

            return dump_to_string(result); 
        }
    } // namespace

    std::unordered_map<std::string, double> get_campaign_info(
                                                            const std::string campaign_tracker_id,
                                                            const std::string tracker_requests_url,
                                                            const std::string tracker_api_key,
                                                            const size_t period)
    {
        std::list<std::string> headers = {"Content-Type: application/json", "Accept: application/json",
                                          "Api-Key: " + tracker_api_key};

        std::unordered_map<std::string, std::string> data = {
                            {"metrics", "[\"clicks\", \"leads\", \"cost\", \"revenue\"]"},
                            {"filters", "[{\"name\": \"campaign_id\", \"operator\": \"EQUALS\", \"expression\":"
                            + campaign_tracker_id + "}]"}};

        // TODO: add range here depending on period (like in binom)
        // auto request_url = http::build_url(tracker_requests_url, params);

        std::string data_encoded = keitaro::dump_to_string(data);

        return {};
    }

}  // namespace keitaro
