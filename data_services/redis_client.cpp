// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#include <string>

#include "acl_cpp/lib_acl.hpp"

#include "redis_client.h"
#include "config_reader.h"

using namespace std;

RedisClient::RedisClient()
{
    auto config = read_config("config.env");
    
    this->host = config["REDIS_HOST"];
    this->port = (size_t)stoi(config["REDIS_PORT"]);

 // 	acl::acl_cpp_init();

	string redis_addr = this->host + ":" + to_string(this->port);
	int conn_timeout = 10, rw_timeout = 10;

	acl::redis_client conn(redis_addr.c_str(), conn_timeout, rw_timeout);

	string key = "test_key";

	acl::redis_string cmd_string(&conn);
    this->test(cmd_string, key);
}

void RedisClient::test(acl::redis_string& cmd, string key)
{
    acl::string val("test_value");

	// call redis-server: SET key value
	if (cmd.set(key.c_str(), val.c_str()) == false)
	{
		cout << "redis set error" << endl;
		return;
	}

	// clear the string buf space
	val.clear();

	// reset the redis command object for reusing it
	cmd.clear();

	// call redis-server: GET key
	if (cmd.get(key.c_str(), val) == false)
        cout << "get key error" << endl;
}
