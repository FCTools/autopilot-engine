// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <utility>

#define BUFFER_SIZE 128

namespace time_
{
    const int seconds_in_day = 24 * 60 * 60;
    namespace
    {
        short get_weekday();
    }  // namespace

    std::string get_now();

    // return day that was "seconds" seconds ago
    std::string get_day(std::size_t seconds);

    std::string get_month(std::size_t seconds);

    std::string get_past_time(const std::size_t seconds);

    std::pair<std::string, std::string> get_range_this_week();

    std::pair<std::string, std::string> get_range_this_year();

    std::pair<std::string, std::string> get_range_last_n_days(std::size_t days_number);

    std::pair<std::string, std::string> get_range_last_month();

    std::pair<std::string, std::string> get_range_this_month();
}  // namespace time_