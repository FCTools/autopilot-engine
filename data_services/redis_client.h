// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once

#include <string>
#include <iostream>

#include "acl_cpp/lib_acl.hpp"

using namespace std;

class RedisClient
{
    size_t port;
    string host;

public:
    RedisClient();

    void test(acl::redis_string& cmd, string key);

};
