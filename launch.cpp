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
#include "propeller_controller.h"

using namespace std;

int main()
{
    const size_t workers_num = 3;

    // launch(workers_num);

    PropellerController controller;
    auto a = controller.get_campaign_cpc(3987182, "2021-01-10 01:00:00", "2021-01-11 23:59:59", "73ddef48a2eeac0987696cc00e437ed07e6d6eaac565ea7f");
    cout << a << endl;

    return EXIT_SUCCESS;
}
