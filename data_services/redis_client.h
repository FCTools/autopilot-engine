// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>
#include <vector>

#include <cpp_redis/cpp_redis>

// there are 2 redis servers: storage redis and actions redis
// Storage redis is used for new bots tasks
// Actions redis is used for campaigns/zones managment - tasks for autopilot-ts 
class RedisClient
{
    size_t storage_port, actions_port;
    std::string storage_host, actions_host, storage_addr, actions_addr;

    bool server_is_available(const std::string host, const size_t port);

public:
    RedisClient();

    // check that redis servers are alive and available
    bool servers_are_available();

    // put new entry into actions redis
    void put_action(std::string key, std::string value);

    // get all new entries from storage redis
    std::vector<std::string> get_updates();

    ~RedisClient();
};
