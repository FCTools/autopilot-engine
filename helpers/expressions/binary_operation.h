// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once
#include <unordered_map>

#include "expression.h"

class BinaryOperation: public Expression
{
    char operation; // & or |
    Expression* left;
    Expression* right;

public:
    BinaryOperation(Expression* l, Expression* r, char operation);

    virtual bool is_true(unordered_map<string, double>& campaign_info);

    virtual ~BinaryOperation();
};
