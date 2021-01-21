// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once
#include <string>
#include <vector>

#include "base_controller.h"

using namespace std;

class PropellerController: public BaseController
{
    const string tracker_requests_url = string(getenv("TRACKER_REQUEST_URL")) + string(getenv("TRACKER_API_KEY"))
                                         + "&date=";
                                         
    const string propeller_requests_url = string(getenv("PROPELLER_REQUESTS_URL"));

public:

    PropellerController();

    vector<string> get_field_values(const string field_name, const string data) const;

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, const size_t period, 
                                                            const string api_key) const;

    virtual ~PropellerController();
};
