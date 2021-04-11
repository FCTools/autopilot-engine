// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

#include <pqxx/pqxx>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "data_services/database_client.h"

namespace database {
namespace {
    std::string _build_connection_string() {
        auto database_name = std::string(getenv("DATABASE_NAME"));
        auto database_host = std::string(getenv("DATABASE_HOST"));
        auto database_port = std::string(getenv("DATABASE_PORT"));
        auto database_user = std::string(getenv("DATABASE_USER"));
        auto database_password = std::string(getenv("DATABASE_PASSWORD"));

        return "dbname=" + database_name +
               " host=" + database_host +
               " port=" + ((database_port.empty())
               ? DEFAULT_PORT : database_port) +
               " user=" + database_user +
               " password=" + database_password;
    }
}  // namespace

    // ts - traffic source
    std::string get_zones_param_number(const std::string ts) {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + ts);

        std::string query("SELECT * from bot_manager_trafficsource WHERE name='"
                          + ts + "'");
        pqxx::result res = xact.exec(query);

        return { (res.begin().begin() + ts_indexes::ZONE_PARAM_INDEX)->c_str()};
    }

    std::string get_ts_name(const size_t ts_id) {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + std::to_string(ts_id));

        std::string query("SELECT * from bot_manager_trafficsource WHERE id='"
                     + std::to_string(ts_id) + "'");
        pqxx::result res = xact.exec(query);

        return { (res.begin().begin() + ts_indexes::NAME_INDEX)->c_str() };
    }

    std::unordered_map<std::string, std::string> get_bot_info(
                                                        const size_t bot_id) {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + std::to_string(bot_id));

        std::string query("SELECT * from bot_manager_bot WHERE ID='"
                          + std::to_string(bot_id) + "'");

        spdlog::get("env_logger")->info("Perform database query: " + query);

        pqxx::result res = xact.exec(query);
        auto bot_info = res.begin().begin();

        std::string ignored_zones = (bot_info +
                                     bot_indexes::IGNORED_ZONES_INDEX)->c_str();
        if (ignored_zones.size() > 0 &&
            ignored_zones.find('\r') != std::string::npos) {
            ignored_zones.erase(remove(ignored_zones.begin(),
                                ignored_zones.end(), '\r'),
                                ignored_zones.end());
        }

        return {{"condition", (bot_info +
                               bot_indexes::CONDITION_INDEX)->c_str()},
                {"period", (bot_info + bot_indexes::PERIOD_INDEX)->c_str()},
                {"action", (bot_info + bot_indexes::ACTION_INDEX)->c_str()},
                {"ts", (bot_info + bot_indexes::TS_INDEX)->c_str()},
                {"ts_api_key", (bot_info + bot_indexes::TS_API_KEY_INDEX)->c_str()},
                {"list_to_add", (bot_info +
                                 bot_indexes::LIST_TO_ADD_INDEX)->c_str()},
                {"ignored_zones", ignored_zones},
                {"client_id", (bot_info +
                                 bot_indexes::CLIENT_ID)->c_str()},
                {"campaigns", (bot_info +
                                 bot_indexes::CAMPAIGNS_INDEX)->c_str()},
                {"tracker", (bot_info +
                                 bot_indexes::TRACKER_INDEX)->c_str()},
                {"tracker_api_key", (bot_info +
                                 bot_indexes::TRACKER_API_KEY_INDEX)->c_str()}};
    }

    std::vector<std::pair<std::string, std::string>> get_bot_campaigns(std::string campaigns_json_str) {
        std::vector<std::pair<std::string, std::string>> result;
    
        // removing spaces
        campaigns_json_str.erase(remove_if(campaigns_json_str.begin(),
                                           campaigns_json_str.end(), isspace),
                                           campaigns_json_str.end());

        const std::string tracker_id_pattern = "\"tracker_id\":\"";
        const size_t tracker_id_pattern_len = tracker_id_pattern.length();
        const std::string source_id_pattern = "\"source_id\":\"";
        const size_t source_id_pattern_len = source_id_pattern.length();

        size_t start_pos = 0, end_pos;

        while (campaigns_json_str.find(source_id_pattern, start_pos) != std::string::npos)
        {
            start_pos = campaigns_json_str.find(source_id_pattern, start_pos) + source_id_pattern_len;
            end_pos = campaigns_json_str.find("\"", start_pos);
            std::string source_id = campaigns_json_str.substr(start_pos, end_pos - start_pos);

            start_pos = campaigns_json_str.find(tracker_id_pattern, start_pos) + tracker_id_pattern_len;
            end_pos = campaigns_json_str.find("\"", start_pos);
            std::string tracker_id = campaigns_json_str.substr(start_pos, end_pos - start_pos);
            
            result.push_back({tracker_id, source_id});
        }

        return result;
    }
}  // namespace database
