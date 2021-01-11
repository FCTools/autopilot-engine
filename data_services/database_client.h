// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

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

    const string DEFAULT_PORT="5432";

public:

    DatabaseClient();

    string get_bot_condition(size_t bot_id);

    size_t get_bot_period(size_t bot_id);

    size_t get_bot_action(size_t bot_id);

    vector<size_t> get_bot_campaigns(size_t bot_id);

    string get_bot_traffic_source(size_t bot_id);

    string get_bot_api_key(size_t bot_id);

};