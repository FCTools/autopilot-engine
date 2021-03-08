// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <unordered_map>

#include "base_controller.h"

BaseController::BaseController() {}

unordered_map<std::string, double> BaseController::calculate_statistics(const double cost, const double revenue, 
                                                                        const int clicks, const int leads) const
{
    return {{"cost", cost}, {"revenue", revenue}, 
            {"clicks", clicks}, {"leads", leads}, 
            {"profit", PROFIT(revenue, cost)}, 
            {"ROI", ROI(revenue, cost)}, {"CR", CR(leads, clicks)},
            {"EPC", EPC(revenue, clicks)}, {"CPC", CPC(cost, clicks)},
            {"CPA", CPA(cost, leads)}};
}

BaseController::~BaseController() {}
