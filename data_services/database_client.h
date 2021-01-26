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

#define DEFAULT_PORT "5432"

// bot row indexes
#define CONDITION_INDEX 3
#define PERIOD_INDEX 9
#define CAMPAIGN_INDEX 2
#define ACTION_INDEX 4
#define TS_INDEX 10
#define API_KEY_INDEX 11

// campaign row indexes
#define TRACKER_ID_INDEX 3
#define SOURCE_ID_INDEX 4

// traffic source row indexes
#define ZONE_PARAM_INDEX 2

using namespace std;

class DatabaseClient
{
    string database_name;
    string database_host;
    string database_port;
    string database_user;
    string database_password;
    string connection_string;

public:

    DatabaseClient();

    unordered_map<string, string> get_bot_info(const size_t bot_id) const;

    vector<size_t> get_bot_campaigns(const size_t bot_id) const;

    pair<size_t, string> get_campaign_ids(const size_t campaign_id);  // returns pair <tracker_id, source_id>

    string get_zones_param_number(string ts);

};