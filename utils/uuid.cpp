// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <random>
#include <sstream>

namespace uuid
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generate_uuid_v4()
    {
        std::stringstream ss;
        int i;
        ss << std::hex;

        for (i = 0; i < 8; i++)
        {
            ss << dis(gen);
        }
        ss << "-";

        for (i = 0; i < 4; i++)
        {
            ss << dis(gen);
        }
        ss << "-4";

        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);

        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";

        for (i = 0; i < 12; i++)
        {
            ss << dis(gen);
        };

        return ss.str();
    }
}  // namespace uuid
