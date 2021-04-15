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

#include "helpers/conditions_parser.h"
#include "helpers/http.h"
#include "helpers/uuid.h"

std::set<std::string> split(std::string source, char delimiter);
