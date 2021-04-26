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

#include "utils/time_.h"

namespace time_
{
    namespace
    {
        short get_weekday()
        {
            std::time_t rawtime;
            std::tm* timeinfo;
            char buffer[BUFFER_SIZE];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            std::time_t epoch = mktime(timeinfo);

            // FIXME: remove 7 here, server time is already UTC
            epoch -= (7 * 60 * 60);  // make time utc and substract seconds value

            timeinfo = localtime(&epoch);
            return timeinfo->tm_wday;
        }
    }  // namespace

    std::string get_day(std::size_t seconds)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[BUFFER_SIZE];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::time_t epoch = mktime(timeinfo);

        // FIXME: remove 7 here, server time is already UTC
        epoch -= (7 * 60 * 60 + (long)seconds);  // make time utc and substract seconds

        timeinfo = localtime(&epoch);

        std::strftime(buffer, BUFFER_SIZE, "%d", timeinfo);

        return std::string(buffer);
    }

    std::string get_month(std::size_t seconds)
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer[BUFFER_SIZE];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::time_t epoch = mktime(timeinfo);

        // FIXME: remove 7 here, server time is already UTC
        epoch -= (7 * 60 * 60 + (long)seconds);  // make time utc and substract seconds

        timeinfo = localtime(&epoch);

        std::strftime(buffer, BUFFER_SIZE, "%m", timeinfo);

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
        char buffer[BUFFER_SIZE];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        std::time_t epoch = mktime(timeinfo);

        // FIXME: remove 7 here, server time is already UTC
        epoch -= (7 * 60 * 60 + (long)seconds);  // make time utc and substract seconds

        timeinfo = localtime(&epoch);

        std::strftime(buffer, BUFFER_SIZE, "%Y-%m-%d %T", timeinfo);

        return std::string(buffer);
    }

    // tested
    std::pair<std::string, std::string> get_range_this_month()
    {
        return {get_past_time((stoi(get_day(0)) - 1) * time_::seconds_in_day), get_now()};
    }

    // tested
    std::pair<std::string, std::string> get_range_last_month()
    {
        std::string last_month_last_day = get_past_time(stoi(get_day(0)) * time_::seconds_in_day);
        auto last_month_last_day_int = stoi(get_day(stoi(get_day(0)) * time_::seconds_in_day));
        
        std::string last_month_first_day = get_past_time((stoi(get_day(0)) - 1) * time_::seconds_in_day +
                                                        last_month_last_day_int * time_::seconds_in_day);
        return {last_month_first_day, last_month_last_day};
    }

    // tested
    std::pair<std::string, std::string> get_range_last_n_days(std::size_t days_number)
    {
        return {get_past_time(days_number * time_::seconds_in_day), get_now()};
    }

    // tested
    std::pair<std::string, std::string> get_range_this_year()
    {
        int days = 0;

        while (stoi(get_month((stoi(get_day(0)) + days) * time_::seconds_in_day)) != 12)
        {
            auto last_month_last_day_int = stoi(get_day((stoi(get_day(0)) + days) * time_::seconds_in_day));
            days += last_month_last_day_int;
        }

        days += stoi(get_day(0)) - 1;

        auto first_day = get_past_time(days * time_::seconds_in_day);

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
        
        return {get_past_time(days_to_substract * time_::seconds_in_day), get_now()};
    }
}  // namespace time_
