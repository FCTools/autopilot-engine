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
#include <vector>
#include <utility>

// statistics formulas
#define PROFIT(revenue, cost) (cost != 0) ? (revenue - cost) : 0.
#define ROI(revenue, cost) (cost != 0) ? ((revenue / cost - 1) * 100) : 0.
#define CR(leads, clicks) (clicks != 0) ? (100 * leads * 1.0 / clicks) : 0.
#define EPC(revenue, clicks) (clicks != 0) ? (revenue / clicks) : 0.
#define CPC(cost, clicks) (clicks != 0) ? (cost / clicks) : 0.
#define CPA(cost, leads) (leads != 0) ? (cost / leads) : 0.

typedef std::vector<std::pair<std::string,
                        std::unordered_map<std::string, double>>> zones_data;

namespace binom {
namespace {
    const std::string tracker_requests_url = std::string(
                                        getenv("TRACKER_REQUEST_URL")) +
                                        std::string(getenv("TRACKER_API_KEY")) +
                                        "&date=";

    // extract zones names from string with all zones info
    std::set<std::string> get_zones_names(const std::string zones_info);

    // extract info about given zone
    std::unordered_map<std::string, double> extract_zone_info(
                                                const std::string zone,
                                                const std::string zones_info);

    std::vector<std::string> get_field_values(const std::string field_name,
                                              const std::string data);

    std::unordered_map<std::string, double> calculate_statistics(
                                                       const double cost,
                                                       const double revenue,
                                                       const int clicks,
                                                       const int leads);
}  // namespace

    std::unordered_map<std::string, double> get_campaign_info(
                                            const size_t campaign_tracker_id,
                                            const size_t period);

    zones_data get_zones_info(const size_t campaign_tracker_id,
                              const size_t period,
                              const std::string zones_param_number,
                              const std::set<std::string>& ignored_zones);
}  // namespace binom
