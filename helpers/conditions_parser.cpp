// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <algorithm>
#include <vector>

#include "conditions/base_condition.h"
#include "helpers/conditions_parser.h"
#include "conditions/conditions.h"
#include "helpers/helpers.h"

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

        // get left and right subconditions
        auto left = source.substr(0, index);
        auto right = source.substr(index + 1, source.length() - index - 1);

        return new ComplexCondition(build(left), build(right), operation);
    }
} // namespace

    // FIXME: this function
    BaseCondition* parse_spa_condition(std::string source) {
        source.erase(remove(source.begin(), source.end(), ' '), source.end());
        source = source.substr(1, source.length() - 2);
        auto conditions_groups = split(source, OR);

        std::vector<BaseCondition*> builded_groups;

        for (auto group : conditions_groups)
        {
            auto elementary_conditions_in_group = split(group, AND);
            std::vector<BaseCondition*> builded_elementary_conditions;

            for (auto condition : elementary_conditions_in_group)
            {
                builded_elementary_conditions.push_back(conditions_parser::build(condition));
            }

            BaseCondition *result = builded_elementary_conditions[0];

            for (std::size_t i = 1; i < builded_elementary_conditions.size(); i++)
            {
                result = new ComplexCondition(result, builded_elementary_conditions[i], AND);
            }

            builded_groups.push_back(result);
        }

        BaseCondition *result_condition = builded_groups[0];

        for (std::size_t i = 1; i < builded_groups.size(); i++)
        {
            result_condition = new ComplexCondition(result_condition, builded_groups[i], OR);
        }
        
        return result_condition;
    }
} // namespace conditions_parser
