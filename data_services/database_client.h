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

using namespace std;

namespace database
{
    string _build_connection_string();

    unordered_map<string, string> get_bot_info(const size_t bot_id);

    vector<size_t> get_bot_campaigns(const size_t bot_id);

    pair<size_t, string> get_campaign_ids(const size_t campaign_id);  // returns pair <tracker_id, source_id>

    string get_zones_param_number(string ts);
}
