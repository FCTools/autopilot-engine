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

namespace redis{
    bool connectable() {
        try {
        cpp_redis::client client;
        client.connect(actions_host, actions_port,
                                        [](const std::string& host,
                                           size_t port,
                                           cpp_redis::connect_state status) {
                if (status == cpp_redis::connect_state::dropped ||
                    status == cpp_redis::connect_state::stopped ||
                    status == cpp_redis::connect_state::failed) {
                    spdlog::get("env_logger")->error("Client disconnected "
                            "from redis on " + host + ":" + std::to_string(port));
                }
            });
        }
        catch(cpp_redis::redis_error) {
            spdlog::get("env_logger")->critical("Can't connect to redis on "
                                                 + actions_addr);
            return false;
        }

        try {
            cpp_redis::client client;
            client.connect(storage_host, storage_port,
                                            [](const std::string& host,
                                               size_t port,
                                               cpp_redis::connect_state status) {
                    if (status == cpp_redis::connect_state::dropped ||
                        status == cpp_redis::connect_state::stopped ||
                        status == cpp_redis::connect_state::failed) {
                        spdlog::get("env_logger")->error("Client disconnected "
                                "from redis on " + host + ":" + std::to_string(port));
                    }
                });
            }
        catch(cpp_redis::redis_error) {
            spdlog::get("env_logger")->critical("Can't connect to redis on "
                                                + storage_addr);
            return false;
        }

        return true;
    }
    void put_action(std::string key, std::string value) {
    cpp_redis::client client;

        try {
            client.connect(actions_host, actions_port,
                                            [](const std::string& host,
                                            size_t port,
                                            cpp_redis::connect_state status) {
                    if (status == cpp_redis::connect_state::dropped ||
                        status == cpp_redis::connect_state::stopped ||
                        status == cpp_redis::connect_state::failed) {
                        spdlog::get("env_logger")->error("Client disconnected "
                                "from redis on " + host + ":" + std::to_string(port));
                    }
                });

            client.set(key, value);
            client.sync_commit();
        }
        catch(cpp_redis::redis_error) {
            spdlog::get("env_logger")->error("Can't connect to redis on "
                                            + actions_addr);
        }

        spdlog::get("env_logger")->info("Put object to redis on "
                                        + actions_addr);
    }
    std::vector<std::string> get_updates() {
        std::vector<std::string> result;

        try {
            cpp_redis::client client;

            client.connect(storage_host, storage_port,
                                            [](const std::string& host,
                                            size_t port,
                                            cpp_redis::connect_state status) {
                    if (status == cpp_redis::connect_state::dropped ||
                        status == cpp_redis::connect_state::stopped ||
                        status == cpp_redis::connect_state::failed) {
                        spdlog::get("env_logger")->error("Client disconnected "
                                "from redis on " + host + ":" + std::to_string(port));
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
                                            + storage_addr);
        }

        return result;
    }
} // namespace redis
