// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <unordered_map>
#include <string>

#include "base_condition.h"

// complex condition means conditions like: (cond_1 & cond_2); (cond_1 | cond_2)
class ComplexCondition: public BaseCondition {
    const char operation;
    BaseCondition* left, *right;

 public:
    ComplexCondition(BaseCondition* l, BaseCondition* r, const char operation);

    virtual bool is_true(
            const std::unordered_map<std::string, double>& campaign_info) const;

    virtual ~ComplexCondition();
};
