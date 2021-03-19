// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>
#include <set>
#include <unordered_map>

#include "base_controller.h"

// controller for PropellerAds
class PropellerController: public BaseController {
    const std::string propeller_requests_url =
                                std::string(getenv("PROPELLER_REQUESTS_URL"));

    std::string zones_param_number;
    const std::string name = "Propeller Ads";

 public:
    PropellerController();

    virtual std::unordered_map<std::string, double> get_campaign_info(
                                        const size_t campaign_tracker_id,
                                        const std::string campaign_source_id,
                                        const size_t period,
                                        const std::string api_key) const;

    virtual zones_data get_zones_info(
                            const size_t campaign_tracker_id,
                            const std::string campaign_source_id,
                            const size_t period,
                            const std::string api_key,
                            const std::set<std::string>& ignored_zones) const;

    virtual ~PropellerController();
};
