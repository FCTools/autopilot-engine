// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

// statistics formulas
#define PROFIT(revenue, cost) (cost != 0) ? (revenue - cost) : 0.
#define ROI(revenue, cost) (cost != 0) ? ((revenue / cost - 1) * 100) : 0.
#define CR(leads, clicks) (clicks != 0) ? (100 * leads / clicks) : 0.
#define EPC(revenue, clicks) (clicks != 0) ? (revenue / clicks) : 0.
#define CPC(cost, clicks) (clicks != 0) ? (cost / clicks) : 0.

using namespace std;

typedef vector<pair<string, unordered_map<string, double>>> zones_data;

// base class for traffic source controller
class BaseController
{

protected:

    unordered_map<string, double> calculate_statistics(const double cost, const double revenue, 
                                                       const int clicks, const int leads) const;

    virtual vector<string> get_zones_names(string zones_info) const = 0;

    virtual unordered_map<string, double> extract_zone_info(string zone, string zones_info) const = 0;

public:

    BaseController();

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, const size_t period, 
                                                            const string api_key) const = 0;

    virtual zones_data get_zones_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                      const size_t period, const string api_key) const = 0;

    virtual ~BaseController();

};
