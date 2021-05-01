// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <set>
#include <string>
#include <vector>
#include <string>
#include <unordered_map>

#include "utils/conditions_parser.h"
#include "utils/http.h"
#include "utils/uuid.h"
#include "utils/time_.h"


std::set<std::string> split(std::string source, char delimiter);

std::string dump_to_string(std::unordered_map<std::string, std::string> &data);

std::vector<std::string> get_field_values(const std::string field_name, const std::string& data);

std::unordered_map<std::string, double> calculate_statistics(
                                                       const double cost,
                                                       const double revenue,
                                                       const int clicks,
                                                       const int leads);

std::string format_log_message(std::string bot_id, std::string task_id);
