// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once
#include <string>
#include <unordered_map>

#include "base_condition.h"

using namespace std;

// elementary condition means conditions like: (clicks > 1000); (revenue < 100) and etc.
class ElementaryCondition: public BaseCondition
{
    string param;
    double value;
    string operation;

public:
    ElementaryCondition();

    ElementaryCondition(string source);

    virtual bool is_true(unordered_map<string, double>& campaign_info);

    virtual ~ElementaryCondition();
};
