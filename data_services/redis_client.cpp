// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#include <string>

#include "redis_client.h"
#include "config_reader.h"

using namespace std;

RedisClient::RedisClient()
{
    auto config = read_config("config.env");
    
    this->host = config["REDIS_HOST"];
    this->port = (size_t)stoi(config["REDIS_PORT"]);

	this->client.connect(this->host, this->port, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
    if (status == cpp_redis::client::connect_state::dropped) {
      std::cout << "client disconnected from " << host << ":" << port << std::endl;
    }
  });
}
