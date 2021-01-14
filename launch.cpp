// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "main_loop.h"


using namespace std;

int main()
{
    const size_t workers_num = 3;

    // shared_ptr<spdlog::logger> file_logger;

    // try 
    // {
    //     file_logger = spdlog::rotating_logger_mt("file_logger", "logs/info_log.log", 1024 * 1024 * 5, 3);
    //     file_logger->set_pattern("[%H:%M:%S %z] [thread %t] [%l] %v");
    //     file_logger->set_level(spdlog::level::debug);
    // }
    // catch (const spdlog::spdlog_ex& ex)
    // {
    //     cout << "Log initialization failed: " << ex.what() << endl;
    //     return EXIT_FAILURE;
    // }

    spdlog::set_pattern("[%t] %+");

    spdlog::info("Engine launched.");
    spdlog::info("Workers number: " + to_string(workers_num));
    
    launch(workers_num);

    return EXIT_SUCCESS;
}
