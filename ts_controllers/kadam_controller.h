// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>

#include "ts_controllers//base_controller.h"


class KadamController: public BaseController {
    std::string zones_param_number;
    const std::string name = "Kadam";
 public:
     KadamController();

    // get campaign info from tracker
    virtual std::unordered_map<std::string, double> get_campaign_info(
                                        const std::string campaign_tracker_id,
                                        const std::string campaign_source_id,
                                        const size_t period,
                                        const std::string ts_api_key,
                                        const std::string tracker,
                                        const std::string tracker_api_key) const;

    // get zones info from tracker for given campaign
    virtual zones_data get_zones_info(
                                const std::string campaign_tracker_id,
                                const std::string campaign_source_id,
                                const size_t period,
                                const std::string ts_api_key,
                                const std::string tracker,
                                const std::string tracker_api_key,
                                const std::set<std::string>& ignored_zones)
                                    const;


     virtual ~KadamController();
};