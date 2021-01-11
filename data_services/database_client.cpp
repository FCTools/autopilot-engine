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

string DatabaseClient::get_bot_field(const size_t bot_id, const size_t index) const
{
    pqxx::connection connection(this->connection_string);
    pqxx::work xact(connection, "Select" + to_string(bot_id));

    string query("SELECT * from bot_manager_bot WHERE ID='" + to_string(bot_id) + "'");
    pqxx::result res = xact.exec(query);

    string result = (res.begin().begin() + index)->c_str();

    return result;
}

string DatabaseClient::get_bot_condition(const size_t bot_id) const
{
    return this->get_bot_field(bot_id, this->condition_index);
}


size_t DatabaseClient::get_bot_period(const size_t bot_id) const
{
    string result = this->get_bot_field(bot_id, this->period_index);
    return (size_t)stoi(result);
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


size_t DatabaseClient::get_bot_action(const size_t bot_id) const
{
    string result = this->get_bot_field(bot_id, this->action_index);
    return (size_t)stoi(result);
}

string DatabaseClient::get_bot_traffic_source(const size_t bot_id) const
{
    return this->get_bot_field(bot_id, this->ts_index);
}

string DatabaseClient::get_bot_api_key(const size_t bot_id) const
{
    return this->get_bot_field(bot_id, this->api_key_index);
}
