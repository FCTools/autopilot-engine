// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <unordered_map>

#include <cstdlib>
#include <string>
#include <vector>
#include <pqxx/pqxx>

using namespace std;

class DatabaseClient
{
    string database_name;
    string database_host;
    string database_port;
    string database_user;
    string database_password;
    string connection_string;

    const string DEFAULT_PORT = "5432";

    const size_t condition_index = 3;
    const size_t period_index = 9;
    const size_t campaign_index = 2;
    const size_t action_index = 4;
    const size_t ts_index = 10;
    const size_t api_key_index = 11;

public:

    DatabaseClient();

    pqxx::row::const_iterator _get_bot_info(const size_t bot_id) const;

    unordered_map<string, string> get_bot_info(const size_t bot_id) const;

    vector<size_t> get_bot_campaigns(const size_t bot_id) const;

    pair<size_t, string> get_campaign_ids(const size_t campaign_id);  // returns pair <tracker_id, source_id>

};