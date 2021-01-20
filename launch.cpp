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
#include "database_client.h"
#include "propeller_controller.h"


using namespace std;

int main(int argc, char** argv)
{
    const size_t workers_num = (size_t)stoi(getenv("POOL_SIZE"));

    spdlog::set_pattern("[%t] %+");

    spdlog::info("Engine launched.");
    spdlog::info("Workers number: " + to_string(workers_num));

    launch(workers_num);

    return EXIT_SUCCESS;
}
