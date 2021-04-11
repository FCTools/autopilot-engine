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
#include <signal.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "main_loop.h"
#include "data_services/redis_client.h"
#include "data_services/database_client.h"
#include "helpers/http.h"

bool env_is_correct() {
    std::ifstream settings_file("env_variables.env");
    std::string var;

    // check environment variables
    while (getline(settings_file, var)) {
        if (!getenv(var.c_str())) {
            spdlog::get("file_logger")->critical("Can't find required environment variable: " + var);
            settings_file.close();
            
            return false;
        }
    }

    settings_file.close();

    // check redis connection
    if(!redis::servers_are_available()) {
        return false;
    }

    // TODO: add postgres checking here and all external
    // resources like tracker and sources apis

    return true;
}

void signal_callback_handler(int signum) {
    std::cout << "Get keyboard interrupt signal. Quit." << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    spdlog::set_pattern("[%t] %+");

    // TODO: remove hardcoded values
    size_t max_files = 10, max_size = 1048576 * 5;
    auto logger = spdlog::rotating_logger_mt("actions_logger", "logs/actions_log.log", max_size, max_files);
    auto env_logger = spdlog::rotating_logger_mt("env_logger", "logs/env_log.log", max_size, max_files);

    spdlog::flush_every(std::chrono::seconds(3));

    env_logger->info("----------------------------------------"
                     "----------------------------------------");
    logger->info("--------------------------------------------"
                 "--------------------------------------------");

    env_logger->info("Start new kernel session.");
    env_logger->info("Start environment checking...");

    // if (!env_is_correct()) {
        // env_logger->critical("Incorrect environment. Quit.");
        // return EXIT_FAILURE;
    // }
    
    env_logger->info("Environment is correct.");

    const std::size_t workers_number = (size_t)std::stoi(getenv("POOL_SIZE"));

    signal(SIGINT, signal_callback_handler);

    env_logger->info("Kernel launched.");
    env_logger->info("Workers number: " + std::to_string(workers_number));

    // start(workers_number);
    auto data = database::get_bot_info(2);
    std::cout << data["period"] << std::endl;
    std::cout << data["action"] << std::endl;
    std::cout << data["condition"] << std::endl;
    std::cout << data["ts"] << std::endl;
    std::cout << data["ts_api_key"] << std::endl;
    std::cout << data["list_to_add"] << std::endl;
    std::cout << data["ignored_zones"] << std::endl;
    std::cout << data["client_id"] << std::endl;
    std::cout << data["campaigns"] << std::endl;
    std::cout << data["tracker"] << std::endl;
    std::cout << data["tracker_api_key"] << std::endl;
    std::cout << data["tracker_url"] << std::endl;

    auto campaigns = database::get_bot_campaigns(data["campaigns"]);

    for (auto campaign : campaigns)
    {
        auto tracker_id = campaign.first;
        auto source_id = campaign.second;

        std::cout << tracker_id << " " << source_id << std::endl;
    }

    return EXIT_SUCCESS;
}
