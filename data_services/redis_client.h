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
#include <vector>

#include <cpp_redis/cpp_redis>

using namespace std;

class RedisClient
{
    size_t storage_port, actions_port;
    string storage_host, actions_host, storage_addr, actions_addr;

public:
    RedisClient();

    // check that redis servers are alive and available
    bool connectable();

    // put new entry into actions redis
    void put_action(string key, string value);

    // get all new entries from storage redisa
    vector<string> get_updates();

    ~RedisClient();
};
