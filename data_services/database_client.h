// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

#include <pqxx/pqxx>

// default port for PostgreSQL
#define DEFAULT_PORT "5432"

// namespace that implements database interface
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
        CAMPAIGNS_INDEX = 15,
        TRACKER_INDEX = 16,
        TRACKER_URL_INDEX = 18,
        TRACKER_API_KEY_INDEX = 17,
        TS_INDEX = 9,
        TS_API_KEY_INDEX = 10,
        IGNORED_ZONES_INDEX = 12,
        LIST_TO_ADD_INDEX = 13,  // list (audience) for evadav
        CLIENT_ID = 14  // client key for mgid, kadam
    };

    // field indexes in row from table 'traffic_sources'
    enum ts_indexes
    {
        NAME_INDEX = 1,
        ZONE_PARAM_INDEX = 2
    };

    std::string build_connection_string();
}  // namespace

    // returns bot info from database
    std::unordered_map<std::string, std::string> get_bot_info(const size_t bot_id);

    // returns campaigns related to given bot
    std::vector<std::pair<std::string, std::string>> get_bot_campaigns(std::string campaigns_json_str);

    // returns traffic source fullname by ID
    std::string get_ts_name(const size_t ts_id);

    // returns binom filtering param number from database
    std::string get_zones_param_number_in_binom(const std::string ts);
}  // namespace database
