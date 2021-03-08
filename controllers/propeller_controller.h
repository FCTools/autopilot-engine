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

using namespace std;

// controller for PropellerAds
class PropellerController: public BaseController
{                                         
    const string propeller_requests_url = string(getenv("PROPELLER_REQUESTS_URL"));

    string zones_param_number;
    const string name = "Propeller Ads";

public:

    PropellerController();

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                                            const size_t period, const string api_key) const;

    virtual zones_data get_zones_info(const size_t campaign_tracker_id, const string campaign_source_id, const size_t period, 
                                      const string api_key, set<string>& ignored_zones) const;

    virtual ~PropellerController();
};
