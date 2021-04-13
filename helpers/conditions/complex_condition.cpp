// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <iostream>

#include "conditions/base_condition.h"
#include "conditions/complex_condition.h"

ComplexCondition::ComplexCondition(
                                BaseCondition* l,
                                BaseCondition* r,
                                const char operation_): BaseCondition(),
                                                        operation(operation_) {
    this->left = l;
    this->right = r;

    if (operation_ != OR && operation_ != AND) {
        throw std::invalid_argument("invalid operation: "
                                    + std::to_string(operation));
    }
}

bool ComplexCondition::is_true(
        const std::unordered_map<std::string, double>& campaign_info) const {
    if (this->operation == OR) {
        return this->left->is_true(campaign_info) ||
        this->right->is_true(campaign_info);
    }

    return this->left->is_true(campaign_info) &&
    this->right->is_true(campaign_info);
}

ComplexCondition::~ComplexCondition() {
    delete this->left;
    delete this->right;
}
