// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <vector>
#include <set>
#include <iostream>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "propeller_controller.h"
#include "base_controller.h"
#include "database_client.h"
#include "tracker_client.h"

using namespace std;


PropellerController::PropellerController() : BaseController()
{
    this->zones_param_number = database::get_zones_param_number(this->name);
}

// get campaign info from tracker
unordered_map<string, double> PropellerController::get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                                                     const size_t period, const string api_key) const
{
    TrackerClient tracker;

    return tracker.get_campaign_info(campaign_tracker_id, period);
}

zones_data PropellerController::get_zones_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                               const size_t period, const string api_key, set<string>& ignored_zones) const
{
    TrackerClient tracker;

    return tracker.get_zones_info(campaign_tracker_id, period, this->zones_param_number, ignored_zones);
}

PropellerController::~PropellerController() {}
