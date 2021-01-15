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
    const string tracker_requests_url = "https://fcttrk.com/?page=Campaigns&api_key=" + string(getenv("TRACKER_API_KEY"))
                                         + "&timezone=+3:00user_group=all&status=all&group=all&traffic_source=all&date=";
    const string propeller_requests_url = "https://ssp-api.propellerads.com/v5/adv/statistics";

public:
    PropellerController();

    double get_field_value(const string field_name, const string data) const;

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, const size_t period, 
                                                            const string api_key) const;

    virtual string get_now() const;

    virtual string get_past_time(const size_t seconds) const;

    virtual ~PropellerController();
};
