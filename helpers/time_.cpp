// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <ctime>

#include "helpers/time_.h"

using namespace std;

std::string get_day(std::size_t seconds)
{
    std::time_t rawtime;
    tm* timeinfo;
    char buffer[256];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::time_t epoch = mktime(timeinfo);
    epoch -= (7 * 60 * 60 + (long)seconds);  // make time

    timeinfo = localtime(&epoch);

    std::strftime(buffer, 256, "%d", timeinfo);

    return std::string(buffer);
}

std::string get_now()
{
    return get_past_time(0);
}

std::string get_past_time(const std::size_t seconds)
{
    std::time_t rawtime;
    tm* timeinfo;
    char buffer[256];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::time_t epoch = mktime(timeinfo);
    epoch -= (7 * 60 * 60 + (long)seconds);  // make time utc and substract seconds value

    timeinfo = localtime(&epoch);

    std::strftime(buffer, 256, "%Y-%m-%d %T", timeinfo);

    return std::string(buffer);
}
