// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once
#include <unordered_map>

#include "base_condition.h"

// complex condition means conditions like: (cond_1 & cond_2); (cond_1 | cond_2)
class ComplexCondition: public BaseCondition
{
    char operation; // & means AND, | means OR
    BaseCondition* left;
    BaseCondition* right;

public:
    ComplexCondition(BaseCondition* l, BaseCondition* r, char operation);

    virtual bool is_true(unordered_map<string, double>& campaign_info);

    virtual ~ComplexCondition();
};
