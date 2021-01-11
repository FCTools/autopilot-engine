// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once
#include <string>

#include "base_controller.h"

using namespace std;

class PropellerController: public BaseController
{
    const string requests_url = "https://ssp-api.propellerads.com/v5/adv/statistics";

public:
    PropellerController();

    double get_field_value(const string field_name, const string data) const;

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_id, const string start_date, 
    const string end_date, const string api_key) const;

    virtual string get_now() const;

    virtual string get_past_time(const size_t seconds) const;

    virtual ~PropellerController();
};
