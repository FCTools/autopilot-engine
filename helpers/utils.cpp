// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <vector>
#include <string>

#include "helpers/http.h"
#include "helpers/utils.h"
#include "tracker_controllers/consts.h"

std::vector<std::string> get_field_values(const std::string field_name, const std::string &data)
{
    std::vector<std::string> result;

    std::string search_pattern = "\"" + field_name + "\":\"";
    size_t pattern_len = search_pattern.length();

    if (data.find(search_pattern) == std::string::npos)
    {
        throw http::IncorrectResponse();
    }

    size_t start_pos = 0, end_pos;

    while (data.find(search_pattern, start_pos) != std::string::npos)
    {
        start_pos = data.find(search_pattern, start_pos) + pattern_len;
        end_pos = data.find("\"", start_pos);
        auto target_str = data.substr(start_pos, end_pos - start_pos);

        result.emplace_back(target_str);
    }

    return result;
}

std::unordered_map<std::string, double> calculate_statistics(const double cost, const double revenue,
                                                                const int clicks, const int leads)
{
    return {{"cost", cost},
            {"revenue", revenue},
            {"clicks", clicks},
            {"leads", leads},
            {"profit", PROFIT(revenue, cost)},
            {"ROI", ROI(revenue, cost)},
            {"CR", CR(leads, clicks)},
            {"EPC", EPC(revenue, clicks)},
            {"CPC", CPC(cost, clicks)},
            {"CPA", CPA(cost, leads)}};
}
