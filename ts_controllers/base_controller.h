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
#include <set>
#include <string>
#include <utility>

// statistics formulas
#define PROFIT(revenue, cost) (revenue - cost)
#define ROI(revenue, cost) (cost != 0) ? ((revenue / cost - 1) * 100) : 0.
#define CR(leads, clicks) (clicks != 0) ? (100 * leads * 1.0 / clicks) : 0.
#define EPC(revenue, clicks) (clicks != 0) ? (revenue / clicks) : 0.
#define CPC(cost, clicks) (clicks != 0) ? (cost / clicks) : 0.
#define CPA(cost, leads) (leads != 0) ? (cost / leads) : 0.

// trackers' names
#define BINOM "Binom"

typedef std::vector<std::pair<std::string,
                    std::unordered_map<std::string, double>>> zones_data;

// base class for traffic source controller
class BaseController {
 protected:
    // calculate campaign (zone) statistics using base data:
    // cost, revenue, clicks and leads
    std::unordered_map<std::string, double> calculate_statistics(
                                                    const double cost,
                                                    const double revenue,
                                                    const int clicks,
                                                    const int leads) const;

 public:
    BaseController();

    // get campaign info from tracker
    virtual std::unordered_map<std::string, double> get_campaign_info(
                                        const std::string campaign_tracker_id,
                                        const std::string campaign_source_id,
                                        const size_t period,
                                        const std::string ts_api_key,
                                        const std::string tracker,
                                        const std::string tracker_requests_url,
                                        const std::string tracker_api_key) const = 0;

    // get zones info from tracker for given campaign
    virtual zones_data get_zones_info(
                                const std::string campaign_tracker_id,
                                const std::string campaign_source_id,
                                const size_t period,
                                const std::string ts_api_key,
                                const std::string tracker,
                                const std::string tracker_requests_url,
                                const std::string tracker_api_key,
                                const std::set<std::string>& ignored_zones)
                                    const = 0;

    virtual ~BaseController();
};
