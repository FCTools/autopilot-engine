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

// statistics formulas
#define PROFIT(revenue, cost) (cost != 0) ? (revenue - cost) : 0.
#define ROI(revenue, cost) (cost != 0) ? ((revenue / cost - 1) * 100) : 0.
#define CR(leads, clicks) (clicks != 0) ? (100 * leads * 1.0 / clicks) : 0.
#define EPC(revenue, clicks) (clicks != 0) ? (revenue / clicks) : 0.
#define CPC(cost, clicks) (clicks != 0) ? (cost / clicks) : 0.
#define CPA(cost, leads) (leads != 0) ? (cost / leads) : 0.

using namespace std;

typedef vector<pair<string, unordered_map<string, double>>> zones_data;

namespace binom
{
    namespace
    {
        const string tracker_requests_url = string(getenv("TRACKER_REQUEST_URL")) + 
                                            string(getenv("TRACKER_API_KEY")) +
                                            "&date=";

        // extract zones names from string with all zones info
        set<string> get_zones_names(const string zones_info);

        // extract info about given zone
        unordered_map<string, double> extract_zone_info(const string zone, const string zones_info);

        vector<string> get_field_values(const string field_name, const string data);

        unordered_map<string, double> calculate_statistics(const double cost, const double revenue, 
                                                           const int clicks, const int leads);
    }


    unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const size_t period);

    zones_data get_zones_info(const size_t campaign_tracker_id, const size_t period, 
                              const string zones_param_number, set<string>& ignored_zones);
}


// class TrackerClient
// {
//     const string tracker_requests_url = string(getenv("TRACKER_REQUEST_URL")) + string(getenv("TRACKER_API_KEY"))
//                                          + "&date=";

//     // extract zones names from string with all zones info
//     virtual set<string> get_zones_names(const string zones_info) const;

//     // extract info about given zone
//     virtual unordered_map<string, double> extract_zone_info(const string zone, const string zones_info) const;

//     vector<string> get_field_values(const string field_name, const string data) const;

//     unordered_map<string, double> calculate_statistics(const double cost, const double revenue, 
//                                                        const int clicks, const int leads) const;

// public:

//     TrackerClient();

//     unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const size_t period) const;

//     zones_data get_zones_info(const size_t campaign_tracker_id, const size_t period, 
//                               const string zones_param_number, set<string>& ignored_zones) const;

//     ~TrackerClient();
// };
