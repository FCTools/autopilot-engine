// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

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
