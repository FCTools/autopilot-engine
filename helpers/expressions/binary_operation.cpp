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

bool BinaryOperation::is_true(BaseController* controller, size_t campaign_id, string start_date, string end_date, string api_key)
{
    if (this->operation == '|')
    {
        return this->left->is_true(controller, campaign_id, start_date, end_date, api_key) || 
        this->right->is_true(controller, campaign_id, start_date, end_date, api_key);
    }

    return this->left->is_true(controller, campaign_id, start_date, end_date, api_key) && 
    this->right->is_true(controller, campaign_id, start_date, end_date, api_key);
}

BinaryOperation::~BinaryOperation()
{
    delete this->left;
    delete this->right;
}
