// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <iostream>
#include <fstream>
#include <vector>

#include "spdlog/spdlog.h"

#include "main_loop.h"
#include "redis_client.h"


using namespace std;

bool env_is_correct()
{
    ifstream settings_file("env_variables.env");
    string var;

    // check all environment variables
    while (getline(settings_file, var))
    {
        if (!getenv(var.c_str()))
        {
            spdlog::critical("Can't find required environment variable: " + var);
            return false;
        }
    }

    settings_file.close();

    // check redis connection
    RedisClient redis;

    if(!redis.connectable())
    {
        return false;
    }

    // TODO: add postgres checking here and all external resources like tracker and sources apis

    return true;
}

int main(int argc, char** argv)
{
    spdlog::set_pattern("[%t] %+");

    if (!env_is_correct())
    {
        spdlog::critical("Quit.");
        return EXIT_FAILURE;
    }

    const size_t workers_number = (size_t)stoi(getenv("POOL_SIZE"));

    spdlog::info("Engine launched.");
    spdlog::info("Workers number: " + to_string(workers_number));

    start(workers_number);

    return EXIT_SUCCESS;
}
