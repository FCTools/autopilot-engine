// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once
#include <string>
#include <iostream>

using namespace std;

class Expression
{
    bool is_binary;

public:
    Expression();

    virtual bool is_true(string traffic_source, size_t campaign_id) = 0;
};
