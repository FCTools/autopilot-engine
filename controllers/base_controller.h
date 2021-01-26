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
#include <list>
#include <vector>
#include <string>
#include <utility>

#define PROFIT(revenue, cost) (revenue - cost)
#define ROI(revenue, cost) ((revenue / cost - 1) * 100)
#define CR(leads, clicks) (100 * leads / clicks)
#define EPC(revenue, clicks) (revenue / clicks)
#define CPC(cost, clicks) (cost / clicks)

using namespace std;

typedef vector<pair<string, unordered_map<string, double>>> zones_data;

class BaseController
{
protected:
    unordered_map<string, double> calculate_statistics(double cost, double revenue, int clicks, int leads) const;

    virtual vector<string> get_zones(string zones_info) const = 0;

    virtual unordered_map<string, double> get_zone_info(string zone, string zones_info) const = 0;

public:

    BaseController();

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, const size_t period, 
                                                            const string api_key) const = 0;

    virtual zones_data get_zones_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                                                 const size_t period, const string api_key) const = 0;

    virtual ~BaseController();
};
