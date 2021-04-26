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

#include "conditions/base_condition.h"

// elementary condition means conditions like: (clicks > 1000);
// (revenue < 100) and etc.
class ElementaryCondition: public BaseCondition {
    std::string param, operation;
    double value;

 public:
    ElementaryCondition();

    explicit ElementaryCondition(const std::string source);

    virtual bool is_true(
        const std::unordered_map<std::string, double>& campaign_info) const;

    virtual ~ElementaryCondition();
};
