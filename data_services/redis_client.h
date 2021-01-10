// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once

#include <string>
#include <iostream>

#include <cpp_redis/cpp_redis>

using namespace std;

class RedisClient
{
    size_t port;
    string host;

    cpp_redis::client client;

public:
    RedisClient();

};
