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

namespace redis {
bool server_is_available(const std::string host, const size_t port)
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

bool servers_are_available() {
    return (redis::server_is_available(redis::actions_host,
                                       redis::actions_port)
            && redis::server_is_available(redis::storage_host,
                                         redis::storage_port));
}

void put_action(std::string key, std::string value) {
    cpp_redis::client client;

    try {
        client.connect(redis::actions_host, redis::actions_port,
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
                                         + redis::actions_addr);
    }

    spdlog::get("env_logger")->info("Put object to redis on "
                                    + redis::actions_addr);
}

std::vector<std::string> get_updates() {
    std::vector<std::string> result;

    try {
        cpp_redis::client client;

        client.connect(redis::storage_host, redis::storage_port,
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

            client.sync_commit();
            client.del(result);
            client.sync_commit();
        }
        catch (cpp_redis::redis_error) {
            spdlog::get("env_logger")->error("Can't connect to redis on "
                                            + storage_addr);
        }

        return result;
    }
} // namespace redis
