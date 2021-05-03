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

// namespace that implements keitaro API
namespace keitaro
{
namespace
{
    std::string calculate_range(std::size_t period);

    zones_data extract_zones_info(std::string &zones_info, const std::set<std::string> &ignored_zones);
} 
    // statistics for whole campaign
    std::unordered_map<std::string, double> get_campaign_info(const std::string campaign_tracker_id,
                                                              const std::string tracker_requests_url,
                                                              const std::string tracker_api_key,
                                                              const size_t period);

    // statistics for campaign splitted by zones
    zones_data get_zones_info(const std::string campaign_tracker_id, const std::string tracker_requests_url,
                              const std::string tracker_api_key, const size_t period,
                              const std::string zones_param_number, const std::set<std::string>& ignored_zones);

}  // namespace keitaro
