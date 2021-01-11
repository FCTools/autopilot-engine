// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once
#include <string>
#include <iostream>

#include "base_controller.h"

using namespace std;

class Expression
{
    bool is_binary;

public:
    Expression();

    virtual bool is_true(BaseController* controller, size_t campaign_id, string start_date, string end_date, string api_key) = 0;

    virtual ~Expression();
};
