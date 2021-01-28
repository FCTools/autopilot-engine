// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

// logical operations
#define AND '&'
#define OR '|' 

// number relations
#define LESS "<"
#define GREATER ">"
#define EQUAL "="
#define LESS_OR_EQUAL "<="
#define GREATER_OR_EQUAL ">="

using namespace std;

// base abstract class for all conditions hierarchy
class BaseCondition
{

public:

    BaseCondition();

    virtual bool is_true(unordered_map<string, double>& campaign_info) const = 0;

    virtual ~BaseCondition();

};
