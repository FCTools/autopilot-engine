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
    namespace
    {
        // field indexes in row from table 'bots'
        enum bot_indexes
        {
            CONDITION_INDEX = 3,
            PERIOD_INDEX = 8,
            CAMPAIGN_INDEX = 2,
            ACTION_INDEX = 4,
            TS_INDEX = 9,
            API_KEY_INDEX = 10,
            IGNORED_ZONES_INDEX = 12
        };
        
        // field indexes in row from table 'campaigns'
        enum campaign_indexes
        {
            TRACKER_ID_INDEX = 3,
            SOURCE_ID_INDEX = 4
        };

        // field indexes in row from table 'traffic_sources'
        enum ts_indexes
        {
            NAME_INDEX = 1,
            ZONE_PARAM_INDEX = 2
        };

        string _build_connection_string();
    }

    // get bot info from database
    unordered_map<string, string> get_bot_info(const size_t bot_id);

    // get campaigns linked with given bot
    vector<size_t> get_bot_campaigns(const size_t bot_id);

    string get_ts_name(const size_t ts_id);

    // get internal campaign id
    // return pair of external ids: id from tracker and id from traffic source
    pair<size_t, string> get_campaign_ids(const size_t campaign_id);

    string get_zones_param_number(const string ts);
}
