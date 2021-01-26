// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <pqxx/pqxx>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "database_client.h"

using namespace std;

namespace database
{
    string _build_connection_string()
    {
        const string database_name = string(getenv("DATABASE_NAME"));
        const string database_host = string(getenv("DATABASE_HOST"));
        const string database_port = string(getenv("DATABASE_PORT"));
        const string database_user = string(getenv("DATABASE_USER"));
        const string database_password = string(getenv("DATABASE_PASSWORD"));

        if (database_port.empty())
        {
            return "dbname=" + database_name + 
                " host=" + database_host + 
                " port=" + DEFAULT_PORT + 
                " user=" + database_user + 
                " password=" + database_password;
        }

        return "dbname=" + database_name + 
            " host=" + database_host + 
            " port=" + database_port + 
            " user=" + database_user + 
            " password=" + database_password;
    }

    string get_zones_param_number(string ts)
    {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + ts);

        string query("SELECT * from bot_manager_trafficsource WHERE name='" + ts + "'");
        pqxx::result res = xact.exec(query);
        return string((res.begin().begin() + ZONE_PARAM_INDEX)->c_str());
    }

    unordered_map<string, string> get_bot_info(const size_t bot_id)
    {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + to_string(bot_id));

        string query("SELECT * from bot_manager_bot WHERE ID='" + to_string(bot_id) + "'");

        spdlog::get("file_logger")->info("Create database query: " + query);

        pqxx::result res = xact.exec(query);
        auto bot_info = res.begin().begin();
        unordered_map<string, string> result;

        result["condition"] = (bot_info + CONDITION_INDEX)->c_str();
        result["period"] = (bot_info + PERIOD_INDEX)->c_str();
        result["campaign_id"] = (bot_info + CAMPAIGN_INDEX)->c_str();
        result["action"] = (bot_info + ACTION_INDEX)->c_str();
        result["ts"] = (bot_info + TS_INDEX)->c_str();
        result["api_key"] = (bot_info + API_KEY_INDEX)->c_str();
        
        return result;
    }

    pair<size_t, string> get_campaign_ids(const size_t campaign_id)
    {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + to_string(campaign_id));

        string query("SELECT * from bot_manager_campaign WHERE ID='" + to_string(campaign_id) + "'");

        spdlog::get("file_logger")->info("Create database query: " + query);

        pqxx::result res = xact.exec(query);

        string tracker_id_str = (res.begin().begin() + TRACKER_ID_INDEX)->c_str();

        string source_id = (res.begin().begin() + SOURCE_ID_INDEX)->c_str();
        size_t tracker_id = (size_t)stoi(tracker_id_str);

        return {tracker_id, source_id};
    }

    vector<size_t> get_bot_campaigns(const size_t bot_id)
    {
        pqxx::connection connection(_build_connection_string());
        pqxx::work xact(connection, "Select" + to_string(bot_id));

        string query("SELECT * from bot_manager_bot_campaigns_list WHERE bot_id='" + to_string(bot_id) + "'");
        pqxx::result res = xact.exec(query);

        vector<size_t> result;

        string campaign_id_str;

        for (auto r = res.begin(); r != res.end(); r++)
        {
            campaign_id_str = (res.begin().begin() + CAMPAIGN_INDEX)->c_str();
            result.push_back((size_t)stoi(campaign_id_str));
        }

        return result;
    }
}
