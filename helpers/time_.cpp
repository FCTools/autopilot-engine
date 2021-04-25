// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <ctime>
#include <cstdint>

#include "helpers/time_.h"

namespace time_
{
    namespace
    {
        short get_weekday()
        {
            std::time_t rawtime;
            std::tm* timeinfo;
            char buffer[256];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            std::time_t epoch = mktime(timeinfo);
            epoch -= (7 * 60 * 60);  // make time utc and substract seconds value

            timeinfo = localtime(&epoch);
            return timeinfo->tm_wday;
        }
    }  // namespace

    std::string get_day(std::size_t seconds)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[256];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::time_t epoch = mktime(timeinfo);
        epoch -= (7 * 60 * 60 + (long)seconds);  // make time

        timeinfo = localtime(&epoch);

        std::strftime(buffer, 256, "%d", timeinfo);

        return std::string(buffer);
    }

    std::string get_month(std::size_t seconds)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[256];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::time_t epoch = mktime(timeinfo);
        epoch -= (7 * 60 * 60 + (long)seconds);  // make time

        timeinfo = localtime(&epoch);

        std::strftime(buffer, 256, "%m", timeinfo);

        return std::string(buffer);
    }

    std::string get_now()
    {
        return get_past_time(0);
    }

    std::string get_past_time(const std::size_t seconds)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[256];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::time_t epoch = mktime(timeinfo);
        epoch -= (7 * 60 * 60 + (long)seconds);  // make time utc and substract seconds value

        timeinfo = localtime(&epoch);

        std::strftime(buffer, 256, "%Y-%m-%d %T", timeinfo);

        return std::string(buffer);
    }

    // tested
    std::pair<std::string, std::string> get_range_this_month()
    {
        return {get_past_time((stoi(get_day(0)) - 1) * 24 * 60 * 60), get_now()};
    }

    // tested
    std::pair<std::string, std::string> get_range_last_month()
    {
        std::string last_month_last_day = get_past_time(stoi(get_day(0)) * 24 * 60 * 60);
        auto last_month_last_day_int = stoi(get_day(stoi(get_day(0)) * 24 * 60 * 60));
        
        std::string last_month_first_day = get_past_time((stoi(get_day(0)) - 1) * 24 * 60 * 60 +
                                                        last_month_last_day_int * 24 * 60 * 60);
        return {last_month_first_day, last_month_last_day};
    }

    // tested
    std::pair<std::string, std::string> get_range_last_n_days(std::size_t days_number)
    {
        return {get_past_time(days_number * 24 * 60 * 60), get_now()};
    }

    // tested
    std::pair<std::string, std::string> get_range_this_year()
    {
        int days = 0;

        while (stoi(get_month((stoi(get_day(0)) + days) * 24 * 60 * 60)) != 12)
        {
            auto last_month_last_day_int = stoi(get_day((stoi(get_day(0)) + days) * 24 * 60 * 60));
            days += last_month_last_day_int;
        }

        days += stoi(get_day(0)) - 1;

        auto first_day = get_past_time(days * 24 * 60 * 60);

        return {first_day, get_now()};
    }

    // tested
    std::pair<std::string, std::string> get_range_this_week()
    {
        short current_weekday = get_weekday();
        short days_to_substract = current_weekday;

        if (current_weekday == 0)
        {
            days_to_substract = 7;
        }

        days_to_substract--;
        
        return {get_past_time(days_to_substract * 24 * 60 * 60), get_now()};
    }
}  // namespace tm
