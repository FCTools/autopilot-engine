// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <set>
#include <unordered_map>
#include <vector>
#include <utility>

#include "tracker_controllers/consts.h"

typedef std::vector<std::pair<std::string,
                        std::unordered_map<std::string, double>>> zones_data;

namespace binom {
namespace {
    zones_data extract_zones_info(std::string &zones_info,
                                  const std::set<std::string> &ignored_zones);

    // get values of one fixed field from json-string like: [{}, {}, ...]
    std::vector<std::string> get_field_values(const std::string field_name,
                                              const std::string& data);

    // calculate statistics metrics using basic params
    std::unordered_map<std::string, double> calculate_statistics(
                                                       const double cost,
                                                       const double revenue,
                                                       const int clicks,
                                                       const int leads);
}  // namespace

    // statistics for whole campaign
    std::unordered_map<std::string, double> get_campaign_info(
                                            const std::string campaign_tracker_id,
                                            const std::string tracker_requests_url,
                                            const std::string tracker_api_key,
                                            const size_t period);

    // statistics for campaign splitted by zones
    zones_data get_zones_info(const std::string campaign_tracker_id,
                              const std::string tracker_requests_url,
                              const std::string tracker_api_key,
                              const size_t period,
                              const std::string zones_param_number,
                              const std::set<std::string>& ignored_zones);
}  // namespace binom
