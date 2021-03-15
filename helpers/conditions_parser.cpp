// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <algorithm>

#include "base_condition.h"
#include "conditions_parser.h"
#include "conditions.h"

namespace conditions_parser{
// private namespace
namespace{
    BaseCondition* build(std::string source) {
    // elementary condition building
        if (source.find(AND) == std::string::npos
        && source.find(OR) == std::string::npos) {
        return new ElementaryCondition(source);
        }

        source = source.substr(1, source.length() - 2);

        int counter = 0;
        size_t index = 0;

        for (char c : source) {
            if (c == '(') {
                counter++;
            } else if (c == ')') {
                counter--;
            }

            if (counter < 0) {
                throw;
            } else if (counter == 0) {
                break;
            }
            index++;
        }
        index++;

        char operation = source[index];

        // left subcondition
        auto left = source.substr(0, index);
        // right subcondition
        auto right = source.substr(index + 1, source.length() - index - 1);

        return new ComplexCondition(this->build(left),
                                this->build(right), operation);
    }
} // namespace
    BaseCondition* parse_condition(std::string source) {
    source.erase(remove(source.begin(), source.end(), ' '), source.end());
    return this->build(source);
    }
} // namespace conditions_parser

