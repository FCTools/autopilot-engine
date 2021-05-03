// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

// statistics formulas
#define PROFIT(revenue, cost) (revenue - cost)
#define ROI(revenue, cost) (cost != 0) ? ((revenue / cost - 1) * 100) : 0.
#define CR(leads, clicks) (clicks != 0) ? (100 * leads * 1.0 / clicks) : 0.
#define EPC(revenue, clicks) (clicks != 0) ? (revenue / clicks) : 0.
#define CPC(cost, clicks) (clicks != 0) ? (cost / clicks) : 0.
#define CPA(cost, leads) (leads != 0) ? (cost / leads) : 0.

// empty campaign info
#define NO_CLICKS "\"no_clicks\""
#define ZONES_PER_PAGE 500

typedef std::vector<std::pair<std::string, std::unordered_map<std::string, double>>> zones_data;

// periods
#define TODAY 1
#define YESTERDAY 2
#define LAST_7_DAYS 3
#define LAST_14_DAYS 4
#define THIS_MONTH 5
#define LAST_MONTH 6
#define THIS_YEAR 7
#define ALL_TIME 9
#define THIS_WEEK 11
#define LAST_2_DAYS 13
#define LAST_3_DAYS 14


