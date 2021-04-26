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

#include "spdlog/spdlog.h"

#include "ts_controllers/propeller_controller.h"
#include "data_services/database_client.h"
#include "tracker_controllers/tracker_controllers.h"

PropellerController::PropellerController() : BaseController()
{
    this->zones_param_number = database::get_zones_param_number(this->name);
}

// get campaign info from tracker
std::unordered_map<std::string, double> PropellerController::get_campaign_info(
                                        const std::string campaign_tracker_id,
                                        const std::string campaign_source_id,
                                        const size_t period,
                                        const std::string ts_api_key,
                                        const std::string tracker,
                                        const std::string tracker_requests_url,
                                        const std::string tracker_api_key) const
{
    if (tracker == BINOM)
    {
        return binom::get_campaign_info(campaign_tracker_id, tracker_requests_url, tracker_api_key, period);
    }
    else if (tracker == KEITARO)
    {
        return keitaro::get_campaign_info(campaign_tracker_id, tracker_requests_url, tracker_api_key, period);
    }

    throw;
}

zones_data PropellerController::get_zones_info(
                            const std::string campaign_tracker_id,
                            const std::string campaign_source_id,
                            const size_t period,
                            const std::string ts_api_key,
                            const std::string tracker,
                            const std::string tracker_requests_url,
                            const std::string tracker_api_key,
                            const std::set<std::string>& ignored_zones) const
{
    if (tracker == BINOM)
    {
        return binom::get_zones_info(campaign_tracker_id, tracker_requests_url, tracker_api_key, period,
                                    this->zones_param_number, ignored_zones);
    }
    else if (tracker == KEITARO)
    {
        return keitaro::get_zones_info(campaign_tracker_id, tracker_requests_url, tracker_api_key, period,
                                       this->zones_param_number, ignored_zones);
    }

    throw;
}

PropellerController::~PropellerController() {}
