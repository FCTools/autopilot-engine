// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>

#include <cpp_redis/cpp_redis>
#include "spdlog/spdlog.h"

#include "redis_client.h"

using namespace std;


RedisClient::RedisClient()
{
    this->storage_host = string(getenv("REDIS_STORAGE_HOST"));
    this->storage_port = (size_t)stoi(string(getenv("REDIS_STORAGE_PORT")));

    this->actions_host = string(getenv("REDIS_ACTIONS_HOST"));
    this->actions_port = (size_t)stoi(string(getenv("REDIS_ACTIONS_PORT")));

    this->actions_addr = this->actions_host + ":" + to_string(this->actions_port);
    this->storage_addr = this->storage_host + ":" + to_string(this->storage_port);
}

bool RedisClient::connectable()
{
    try
    {
        cpp_redis::client client;
        client.connect(this->actions_host, this->actions_port, [](const string& host, size_t port, cpp_redis::connect_state status) 
            {
                if (status == cpp_redis::connect_state::dropped || 
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) 
                {
                    spdlog::error("Client disconnected from redis on " + host + ":" + to_string(port));
                }
            });
    }
    catch(cpp_redis::redis_error)
    {
        spdlog::critical("Can't connect to redis on " + this->actions_addr);
        return false;
    }
    
    try
    {
        cpp_redis::client client;
        client.connect(this->storage_host, this->storage_port, [](const string& host, size_t port, cpp_redis::connect_state status) 
            {
                if (status == cpp_redis::connect_state::dropped || 
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) 
                {
                    spdlog::error("Client disconnected from redis on " + host + ":" + to_string(port));
                }
            });
    }
    catch(cpp_redis::redis_error)
    {
        spdlog::critical("Can't connect to redis on " + this->storage_addr);
        return false;
    }

    return true;
}

void RedisClient::put_action(string key, string value)
{
    cpp_redis::client client;

    try
    {
        client.connect(this->actions_host, this->actions_port, [](const string& host, size_t port, 
                                                                  cpp_redis::connect_state status)
            {
                if (status == cpp_redis::connect_state::dropped || 
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) 
                {
                    spdlog::error("Client disconnected from redis on " + host + ":" + to_string(port));
                }
            });

        client.set(key, value);
        client.sync_commit();
    }
    catch(cpp_redis::redis_error)
    {
        spdlog::error("Can't connect to redis on " + this->actions_addr);
    }

    spdlog::info("Put object to redis on " + this->actions_addr);
}

vector<string> RedisClient::get_updates()
{
    vector<string> result;

    try
    {
        cpp_redis::client client;

        client.connect(this->storage_host, this->storage_port, [](const string& host, size_t port, 
                                                                  cpp_redis::connect_state status) 
            {
                if (status == cpp_redis::connect_state::dropped || 
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) 
                {
                    spdlog::error("Client disconnected from redis on " + host + ":" + to_string(port));
                }
            });

        cpp_redis::client::reply_callback_t reply;

        client.keys("*", [&result](cpp_redis::reply &reply) {
            if(reply.is_array()) 
            {
                for(auto& key : reply.as_array()) 
                {
                    auto x = key.as_string();
                    result.push_back(x);
                }
            }   
        });
        
        client.sync_commit();
        client.del(result);
        client.sync_commit();
    }
    catch(cpp_redis::redis_error)
    {
        spdlog::error("Can't connect to redis on " + this->storage_addr);
    }

    return result;
}

RedisClient::~RedisClient() {}
