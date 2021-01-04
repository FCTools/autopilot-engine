// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once
#include <string>

#include "expression.h"
#include "base_controller.h"
using namespace std;


class ElementaryCondition: public Expression
{
    string param;
    double value;
    string operation;

public:
    ElementaryCondition();

    ElementaryCondition(string source);

    double get_fact_value(BaseController* controller, size_t campaign_id, string start_date, string end_date);

    virtual bool is_true(BaseController* controller, size_t campaign_id, string start_date, string end_date);

    virtual ~ElementaryCondition();
};
