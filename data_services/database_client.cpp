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

DatabaseClient::DatabaseClient() 
{
    this->database_name = string(getenv("DATABASE_NAME"));
    this->database_host = string(getenv("DATABASE_HOST"));
    this->database_port = string(getenv("DATABASE_PORT"));
    this->database_user = string(getenv("DATABASE_USER"));
    this->database_password = string(getenv("DATABASE_PASSWORD"));

    if (this->database_port.empty())
    {
        this->database_port = this->DEFAULT_PORT;
    }

    this->connection_string = "dbname=" + this->database_name + 
                              " host=" + this->database_host + 
                              " port=" + this->database_port + 
                              " user=" + this->database_user + 
                              " password=" + this->database_password;
}

pqxx::row::const_iterator DatabaseClient::_get_bot_info(const size_t bot_id) const
{
    pqxx::connection connection(this->connection_string);
    pqxx::work xact(connection, "Select" + to_string(bot_id));

    string query("SELECT * from bot_manager_bot WHERE ID='" + to_string(bot_id) + "'");

    spdlog::info("Create database query: " + query);

    pqxx::result res = xact.exec(query);
    return res.begin().begin();
}

unordered_map<string, string> DatabaseClient::get_bot_info(const size_t bot_id) const
{
    auto bot_info = this->_get_bot_info(bot_id);
    unordered_map<string, string> result;

    result["condition"] = (bot_info + this->condition_index)->c_str();
    result["period"] = (bot_info + this->period_index)->c_str();
    result["campaign_id"] = (bot_info + this->campaign_index)->c_str();
    result["action"] = (bot_info + this->action_index)->c_str();
    result["ts"] = (bot_info + this->ts_index)->c_str();
    result["api_key"] = (bot_info + this->api_key_index)->c_str();

    return result;
}

pair<size_t, string> DatabaseClient::get_campaign_ids(const size_t campaign_id)
{
    pqxx::connection connection(this->connection_string);
    pqxx::work xact(connection, "Select" + to_string(campaign_id));

    string query("SELECT * from bot_manager_campaign WHERE ID='" + to_string(campaign_id) + "'");

    spdlog::info("Create database query: " + query);

    pqxx::result res = xact.exec(query);

    const size_t tracker_id_index = 0;  // set true value
    const size_t source_id_index = 1;  // set true value

    string tracker_id_str = (res.begin().begin() + tracker_id_index)->c_str();
    string source_id = (res.begin().begin() + source_id_index)->c_str();

    size_t tracker_id = (size_t)stoi(tracker_id_str);

    return {tracker_id, source_id};
}

vector<size_t> DatabaseClient::get_bot_campaigns(const size_t bot_id) const
{
    pqxx::connection connection(this->connection_string);
    pqxx::work xact(connection, "Select" + to_string(bot_id));

    string query("SELECT * from bot_manager_bot_campaigns_list WHERE bot_id='" + to_string(bot_id) + "'");
    pqxx::result res = xact.exec(query);

    vector<size_t> result;

    string campaign_id_str;

    for (auto r = res.begin(); r != res.end(); r++)
    {
        campaign_id_str = (res.begin().begin() + campaign_index)->c_str();
        result.push_back((size_t)stoi(campaign_id_str));
    }

    return result;
}
