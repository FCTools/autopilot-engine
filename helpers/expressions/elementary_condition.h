// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once
#include <string>
#include <unordered_map>

#include "expression.h"

using namespace std;


class ElementaryCondition: public Expression
{
    string param;
    double value;
    string operation;

public:
    ElementaryCondition();

    ElementaryCondition(string source);

    virtual bool is_true(unordered_map<string, double>& campaign_info);

    virtual ~ElementaryCondition();
};
