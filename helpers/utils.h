// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <vector>
#include <string>
#include <unordered_map>

// get values of one fixed field from json-string like: [{}, {}, ...]
std::vector<std::string> get_field_values(const std::string field_name, const std::string& data);

std::unordered_map<std::string, double> calculate_statistics(
                                                       const double cost,
                                                       const double revenue,
                                                       const int clicks,
                                                       const int leads);
