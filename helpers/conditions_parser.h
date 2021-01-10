// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

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

