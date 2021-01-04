// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once
#include <string>

#include "expressions/expression.h"
using namespace std;


class ConditionsParser
{
    Expression* build(string source);

public:
    ConditionsParser();

    Expression* parse(string source);

};

