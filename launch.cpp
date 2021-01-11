// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>


#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <locale>

#include "main_loop.h"

using namespace std;

int main()
{
    const size_t workers_num = 10;

    launch(workers_num);

    return EXIT_SUCCESS;
}
