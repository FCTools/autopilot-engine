// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <fstream>
#include <string>
#include <sstream>
#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "base_controller.h"

using namespace std;

BaseController::BaseController() {}

unordered_map<string, double> BaseController::calculate_statistics(double cost, double revenue, 
                                                                   int clicks, int leads) const
{
    unordered_map<string, double> result = {{"ROI", 0.}, {"CR", 0.}, {"EPC", 0.}, {"CPC", 0.}};
    
    result["cost"] = cost;
    result["revenue"] = revenue;
    result["clicks"] = clicks;
    result["leads"] = leads;
    result["profit"] = PROFIT(revenue, cost);

    if (cost > 0)
    {
        result["ROI"] = ROI(revenue, cost);
    }
    
    if (clicks > 0)
    {
        result["CR"] = CR(leads, clicks);
        result["EPC"] = EPC(revenue, clicks);
        result["CPC"] = CPC(cost, clicks);
    }
    
    return result;
}

BaseController::~BaseController() {}
