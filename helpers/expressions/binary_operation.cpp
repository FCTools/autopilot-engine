// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#include <iostream>

#include "binary_operation.h"
#include "base_controller.h"
using namespace std;

BinaryOperation::BinaryOperation(Expression* l, Expression* r, char operation): Expression()
{
    this->left = l;
    this->right = r;

    if (operation != '|' && operation != '&')
    {
        throw invalid_argument("invalid operation: " + to_string(operation));
    }
    this->operation = operation;
}

bool BinaryOperation::is_true(unordered_map<string, double>& campaign_info)
{
    if (this->operation == '|')
    {
        return this->left->is_true(campaign_info) || 
        this->right->is_true(campaign_info);
    }

    return this->left->is_true(campaign_info) && 
    this->right->is_true(campaign_info);
}

BinaryOperation::~BinaryOperation()
{
    delete this->left;
    delete this->right;
}
