// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <vector>

#include "redis_client.h"
#include "config_reader.h"

using namespace std;

RedisClient::RedisClient()
{
    auto config = read_config("config.env");
    
    this->storage_host = config["REDIS_STORAGE_HOST"];
    this->storage_port = (size_t)stoi(config["REDIS_STORAGE_PORT"]);

    this->actions_host = config["REDIS_ACTIONS_HOST"];
    this->actions_port = (size_t)stoi(config["REDIS_ACTIONS_PORT"]);
}

void RedisClient::put(string key, string value)
{
    cpp_redis::client client;

	client.connect(this->actions_host, this->actions_port, [](const string& host, size_t port, cpp_redis::client::connect_state status) {
    if (status == cpp_redis::client::connect_state::dropped) {
        cout << "client disconnected from " << host << ":" << port << endl;
    }
  	});

	client.set(key, value);
	client.sync_commit();
}

vector<string> RedisClient::_get_keys()
{
    vector<string> result;
    string tmp = "";

    cpp_redis::client client;

	client.connect(this->actions_host, this->actions_port, [](const string& host, size_t port, cpp_redis::client::connect_state status) {
    if (status == cpp_redis::client::connect_state::dropped) {
        cout << "client disconnected from " << host << ":" << port << endl;
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

    return result;
}

vector<string> RedisClient::get_updates()
{
    cpp_redis::client client;

	client.connect(this->actions_host, this->actions_port, [](const string& host, size_t port, cpp_redis::client::connect_state status) {
    if (status == cpp_redis::client::connect_state::dropped) {
        cout << "client disconnected from " << host << ":" << port << endl;
    }
  	});

    auto keys_list = this->_get_keys();
    vector<string> result;

    for (string& key: keys_list)
    {
        client.get(key, [&result](cpp_redis::reply& reply) {
            result.push_back(reply.as_string());
        });
    }

    client.sync_commit();

    return result;
}

RedisClient::~RedisClient()
{
	// this->client.client_kill(this->host, this->port, [](cpp_redis::reply& reply) {
    //   std::cout << reply << std::endl; //! OK
    // });
}
