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
#include "spdlog/sinks/rotating_file_sink.h"

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
            spdlog::get("file_logger")->critical("Can't find required environment variable: " + var);
            settings_file.close();
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

    auto max_size = 1048576 * 5;
    auto max_files = 10;
    auto logger = spdlog::rotating_logger_mt("file_logger", "logs/info_log.log", max_size, max_files);

    logger->info("---------------------");
    logger->info("Start new kernel session.");

    logger->info("Start environment checking...");
    if (!env_is_correct())
    {
        logger->critical("Quit.");
        return EXIT_FAILURE;
    }
    logger->info("Environment is correct.");

    const size_t workers_number = (size_t)stoi(getenv("POOL_SIZE"));

    logger->info("Engine launched.");
    logger->info("Workers number: " + to_string(workers_number));

    start(workers_number);

    return EXIT_SUCCESS;
}
