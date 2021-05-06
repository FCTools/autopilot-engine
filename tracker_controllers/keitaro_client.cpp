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
#include "utils/utils.h"

namespace keitaro
{
    namespace
    {
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

        zones_data extract_zones_info(std::string &zones_info, const std::set<std::string> &ignored_zones)
        {
            zones_data result;

            std::string search_pattern = "\"source\":\"";
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

                double cost = stod((*(get_field_values("cost", zone_info).begin())));
                double revenue = stod((*(get_field_values("revenue", zone_info).begin())));
                double clicks = stod((*(get_field_values("clicks", zone_info).begin())));
                int leads = stoi((*(get_field_values("leads", zone_info).begin())));

                auto statistics = calculate_statistics(cost, revenue, clicks, leads);

                if (!name.empty())
                {
                    result.push_back({name, statistics});
                }
            }

            return result;
        }

    } // namespace

    // TODO: add task_id to the arguments
    std::unordered_map<std::string, double> get_campaign_info(const std::string campaign_tracker_id,
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

        std::string data_encoded = dump_to_string(data);

        size_t tries = DEFAULT_REQUEST_TRIES;
        std::string campaign_info;

        std::string request_url = tracker_requests_url + "admin_api/v1/report/build";

        while (tries != 0)
        {
            spdlog::get("actions_logger")->info("Perform request: " + request_url);

            campaign_info = http::make_request(headers, data_encoded, request_url, "POST");
            // TODO: log response to database with task_id, requests_url, headers, body and type (POST, GET, etc.)

            if (campaign_info.size() == 0)
            {
                spdlog::get("actions_logger")->error("Error while trying to make request (or empty result)");
                tries--;

                // TODO: remove hardcoded value: 5 seconds
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            try
            {
                double cost = stod(get_field_values("cost", campaign_info)[0]);
                double revenue = stod(get_field_values("revenue", campaign_info)[0]);
                double clicks = stod(get_field_values("clicks", campaign_info)[0]);
                double leads = stod(get_field_values("leads", campaign_info)[0]);

                return calculate_statistics(cost, revenue, clicks, leads);
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

    // TODO: add task_id to the arguments
    zones_data get_zones_info(const std::string campaign_tracker_id, const std::string tracker_requests_url,
                              const std::string tracker_api_key, const size_t period,
                              const std::string zones_param_number, const std::set<std::string>& ignored_zones)
    {
        std::list<std::string> headers = {"Content-Type: application/json", "Accept: application/json",
                                          "Api-Key: " + tracker_api_key};

        std::string requests_url = tracker_requests_url + "admin_api/v1/report/build";
        
        std::string tmp_zones_info;
        zones_data zones_info, zones_page;

        std::size_t offset = 0;
        std::size_t tries = DEFAULT_REQUEST_TRIES;

        std::unordered_map<std::string, std::string> data = {
                                    {"metrics", "[\"leads\", \"cost\", \"revenue\", \"clicks\"]"},
                                    {"filters", "[{\"name\": \"campaign_id\", \"operator\": \"EQUALS\", \"expression\":"
                                        + campaign_tracker_id + "}]"},
                                    {"grouping", "[\"source\"]"},
                                    {"range", keitaro::calculate_range(period)},
                                    {"limit", std::to_string(ZONES_PER_PAGE)},
                                    {"offset", std::to_string(offset)}};

        while (tries != 0)
        {
            try
            {
                while (true)
                {
                    data["offset"] = offset;
                    std::string data_encoded = dump_to_string(data);

                    tmp_zones_info = http::make_request(headers, data_encoded, requests_url, http::POST);
                    // TODO: log response to database with task_id, requests_url, headers, body and type (POST, GET, etc.)
                    // and maybe add field "description" to each entry
                    
                    if (tmp_zones_info == "null")
                    {
                        break;
                    }

                    spdlog::get("actions_logger")->info("Perform request: " + requests_url);

                    zones_page = keitaro::extract_zones_info(tmp_zones_info, ignored_zones);
                    zones_info.insert(zones_info.end(), zones_page.begin(), zones_page.end());

                    if (zones_page.size() < ZONES_PER_PAGE)
                    {
                        break;
                    }
                    zones_page.clear();
                    offset += ZONES_PER_PAGE;
                }

                return zones_info;
            } 
            catch (http::RequestError)
            {
                spdlog::get("actions_logger")->error("RequestError while trying to get zones info from keitaro."
                                                     " Campaign id: " + campaign_tracker_id);
            }

            tries--;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        if (tmp_zones_info.size() == 0)
        {
            spdlog::get("actions_logger")->error("Error or empty result while trying to get zones info "
                                                 "from keitaro. Campaign id: " + campaign_tracker_id);
            return {};
        }

        return zones_info;
    }

}  // namespace keitaro
