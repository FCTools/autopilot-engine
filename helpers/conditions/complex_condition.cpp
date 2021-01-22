// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <iostream>

#include "complex_condition.h"
#include "base_controller.h"

using namespace std;

ComplexCondition::ComplexCondition(BaseCondition* l, BaseCondition* r, char operation): BaseCondition()
{
    this->left = l;
    this->right = r;

    if (operation != '|' && operation != '&')
    {
        throw invalid_argument("invalid operation: " + to_string(operation));
    }
    this->operation = operation;
}

bool ComplexCondition::is_true(unordered_map<string, double>& campaign_info)
{
    if (this->operation == '|')
    {
        return this->left->is_true(campaign_info) || 
        this->right->is_true(campaign_info);
    }

    return this->left->is_true(campaign_info) && 
    this->right->is_true(campaign_info);
}

ComplexCondition::~ComplexCondition()
{
    delete this->left;
    delete this->right;
}
