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

        std::string calculate_range(std::size_t period)
        {
            std::unordered_map<std::string, std::string> result = {{"timezone", "Europe/Madrid"}};

            if (period == ALL_TIME)
            {
                result["interval"] = "all_time";
                return dump_to_string(result);
            }

            std::pair<std::string, std::string> range;

            switch (period)
            {
                case TODAY:
                    {
                        std::string today_ = time_::get_now();
                        range = {today_, today_};
                    }
                    break;
                case YESTERDAY:
                    {
                        std::string yesterday_ = time_::get_past_time(24 * 60 * 60);
                        range = {yesterday_, yesterday_};
                    }
                    break;
                case LAST_7_DAYS:
                    range = time_::get_range_last_n_days(7);
                    break;
                case LAST_14_DAYS:
                    range = time_::get_range_last_n_days(14);
                    break;
                case THIS_MONTH:
                    range = time_::get_range_this_month();
                    break;
                case LAST_MONTH:
                    range = time_::get_range_last_month();
                    break;
                case THIS_YEAR:
                    range = time_::get_range_this_year();
                    break;
                case THIS_WEEK:
                    range = time_::get_range_this_week();
                    break;
                case LAST_2_DAYS:
                    range = time_::get_range_last_n_days(2);
                    break;
                case LAST_3_DAYS:
                    range = time_::get_range_last_n_days(3);
                    break;
                default:
                    break;
            }

            result["from"] = range.first;
            result["to"] = range.second;

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

        data["range"] = keitaro::calculate_range(period);

        // TODO: add range here depending on period (like in binom)
        // auto request_url = http::build_url(tracker_requests_url, params);

        std::string data_encoded = keitaro::dump_to_string(data);

        

        return {};
    }

}  // namespace keitaro
