// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>

#include "redis_client.h"
#include "config_reader.h"

using namespace std;

RedisClient::RedisClient()
{
    auto config = read_config("config.env");
    
    this->host = config["REDIS_HOST"];
    this->port = (size_t)stoi(config["REDIS_PORT"]);

}

void RedisClient::put(string key, string value)
{
    cpp_redis::client client;

	client.connect(this->host, this->port, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
    if (status == cpp_redis::client::connect_state::dropped) {
      std::cout << "client disconnected from " << host << ":" << port << std::endl;
    }
  	});

	client.set(key, value);
	client.sync_commit();
}

RedisClient::~RedisClient()
{
	// this->client.client_kill(this->host, this->port, [](cpp_redis::reply& reply) {
    //   std::cout << reply << std::endl; //! OK
    // });
}
