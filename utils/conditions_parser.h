// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>

#include "conditions/base_condition.h"


namespace conditions_parser
{
namespace
{
    BaseCondition* build(std::string source);
} // namespace
    BaseCondition* parse_spa_condition(std::string source);
} // namespace conditions_parser

