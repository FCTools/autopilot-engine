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

namespace database {
namespace {
    // field indexes in row from table 'bots'
    enum bot_indexes {
        CONDITION_INDEX = 3,
        PERIOD_INDEX = 8,
        CAMPAIGN_INDEX = 2,
        ACTION_INDEX = 4,
        CAMPAIGNS_INDEX = 15,
        TS_INDEX = 9,
        API_KEY_INDEX = 10,
        IGNORED_ZONES_INDEX = 12,
        LIST_TO_ADD_INDEX = 13,  // list (audience) for evadav
        CLIENT_ID = 14  // client key for mgid
    };

    // field indexes in row from table 'campaigns'
    enum campaign_indexes {
        TRACKER_ID_INDEX = 3,
        SOURCE_ID_INDEX = 4
    };

    // field indexes in row from table 'traffic_sources'
    enum ts_indexes {
        NAME_INDEX = 1,
        ZONE_PARAM_INDEX = 2
    };

    std::string build_connection_string();
}  // namespace

    // get bot info from database
    std::unordered_map<std::string, std::string> get_bot_info(
                                                        const size_t bot_id);

    // get campaigns linked with given bot
    std::vector<std::pair<std::string, std::string>> get_bot_campaigns(
                                            std::string campaigns_json_str);

    std::string get_ts_name(const size_t ts_id);

    std::string get_zones_param_number(const std::string ts);
}  // namespace database
