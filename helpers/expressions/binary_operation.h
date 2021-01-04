// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once

#include "expression.h"

class BinaryOperation: public Expression
{
    char operation; // & or |
    Expression* left;
    Expression* right;

public:
    BinaryOperation(Expression* l, Expression* r, char operation);

    virtual bool is_true(string traffic_source, size_t campaign_id);

    ~BinaryOperation();
};
