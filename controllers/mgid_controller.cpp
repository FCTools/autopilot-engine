// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include "mgid_controller.h"
#include "database_client.h"
#include "binom_client.h"


MgidController::MgidController() : BaseController() {
    this->zones_param_number = database::get_zones_param_number(this->name);
}

// get campaign info from tracker
std::unordered_map<std::string, double> MgidController::get_campaign_info(
                                        const std::string campaign_tracker_id,
                                        const std::string campaign_source_id,
                                        const size_t period,
                                        const std::string ts_api_key,
                                        const std::string tracker,
                                        const std::string tracker_api_key) const {
    return binom::get_campaign_info(campaign_tracker_id, period);
}

zones_data MgidController::get_zones_info(
                                    const std::string campaign_tracker_id,
                                    const std::string campaign_source_id,
                                    const size_t period,
                                    const std::string ts_api_key,
                                    const std::string tracker,
                                    const std::string tracker_api_key,
                                    const std::set<std::string>& ignored_zones)
                                            const {
    return binom::get_zones_info(campaign_tracker_id, period,
                                 this->zones_param_number, ignored_zones);
}

MgidController::~MgidController() {}
