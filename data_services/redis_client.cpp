// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <vector>
#include <unordered_map>

#include <cpp_redis/cpp_redis>
#include "spdlog/spdlog.h"

#include "redis_client.h"

// TODO: make redis namespace
RedisClient::RedisClient() {
    this->storage_host = std::string(getenv("REDIS_STORAGE_HOST"));
    this->storage_port =
                        (size_t)stoi(std::string(getenv("REDIS_STORAGE_PORT")));

    this->actions_host = std::string(getenv("REDIS_ACTIONS_HOST"));
    this->actions_port =
                        (size_t)stoi(std::string(getenv("REDIS_ACTIONS_PORT")));

    this->actions_addr = this->actions_host + ":"
                         + std::to_string(this->actions_port);
    this->storage_addr = this->storage_host + ":"
                         + std::to_string(this->storage_port);
}

bool RedisClient::server_is_available(const std::string host, const size_t port)
{
    try {
        cpp_redis::client client;
        client.connect(host, port, [](const std::string& host_, size_t port_,
                                      cpp_redis::connect_state status) {
                if (status == cpp_redis::connect_state::dropped ||
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) {
                    spdlog::get("env_logger")->error(
                        "Client disconnected from redis on " + host_
                        + ":" + std::to_string(port_));
                }
            });
    }
    catch(cpp_redis::redis_error) {
        spdlog::get("env_logger")->critical(
            "Can't connect to redis on " + host + ":" + std::to_string(port));
        return false;
    }

    return true;
}

bool RedisClient::servers_are_available() {
    return (this->server_is_available(this->actions_host,
                                      this->actions_port)
            && this->server_is_available(this->storage_host,
                                         this->storage_port));
}

void RedisClient::put_action(std::string key, std::string value) {
    cpp_redis::client client;

    try {
        client.connect(this->actions_host, this->actions_port,
                                        [](const std::string& host,
                                           size_t port,
                                           cpp_redis::connect_state status) {
                if (status == cpp_redis::connect_state::dropped ||
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) {
                    spdlog::get("env_logger")->error("Client disconnected "
                            "from redis on " + host + ":"
                            + std::to_string(port));
                }
            });

        client.set(key, value);
        client.sync_commit();
    }
    catch(cpp_redis::redis_error) {
        spdlog::get("env_logger")->error("Can't connect to redis on "
                                         + this->actions_addr);
    }

    spdlog::get("env_logger")->info("Put object to redis on "
                                    + this->actions_addr);
}

std::vector<std::string> RedisClient::get_updates() {
    std::vector<std::string> result;

    try {
        cpp_redis::client client;

        client.connect(this->storage_host, this->storage_port,
                                        [](const std::string& host,
                                           size_t port,
                                           cpp_redis::connect_state status) {
                if (status == cpp_redis::connect_state::dropped ||
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) {
                    spdlog::get("env_logger")->error("Client disconnected "
                            "from redis on " + host + ":"
                            + std::to_string(port));
                }
            });

        cpp_redis::client::reply_callback_t reply;

        client.keys("*", [&result](cpp_redis::reply &reply) {
            if (reply.is_array()) {
                for (auto& key : reply.as_array()) {
                    result.emplace_back(key.as_string());
                }
            }
        });

        client.sync_commit();
        client.del(result);
        client.sync_commit();
    }
    catch (cpp_redis::redis_error) {
        spdlog::get("env_logger")->error("Can't connect to redis on "
                                         + this->storage_addr);
    }

    return result;
}

RedisClient::~RedisClient() {}
