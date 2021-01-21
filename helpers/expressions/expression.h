// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

class Expression
{
    bool is_binary;

public:
    Expression();

    virtual bool is_true(unordered_map<string, double>& campaign_info) = 0;

    virtual ~Expression();
};
