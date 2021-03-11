// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <unordered_map>

#include "base_condition.h"
#include "elementary_condition.h"

ElementaryCondition::ElementaryCondition(): BaseCondition() {}

ElementaryCondition::ElementaryCondition(std::string source): BaseCondition() {
    // removing external brackets
    source = source.substr(1, source.length() - 2);
    
    if (source.find(EQUAL) != std::string::npos
         && (source.find(LESS) != std::string::npos
         || source.find(GREATER) != std::string::npos)) {
        size_t relation_index = source.find(EQUAL);

        this->operation = source.substr(relation_index - 1, 2);
        this->param = source.substr(0, relation_index - 1);
        this->value = stod(source.substr(relation_index + 1,
                                         source.length() - relation_index - 1));
    }
    else {
        size_t relation_index = source.find(LESS);
        
        if (relation_index == std::string::npos) {
            relation_index = source.find(GREATER);
        }

        this->operation = source.substr(relation_index, 1);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 1,
                                        source.length() - relation_index - 1));
    }
}


bool ElementaryCondition::is_true(
        const std::unordered_map<std::string, double>& campaign_info) const {
    double fact_value = campaign_info.find(this->param)->second;

    if (this->operation == LESS) {
        return fact_value < this->value;
    }
    else if (this->operation == LESS_OR_EQUAL) {
        return fact_value <= this->value;
    }
    else if (this->operation == GREATER) {
        return fact_value > this->value;
    }
    else if (this->operation == GREATER_OR_EQUAL) {
        return fact_value >= this->value;
    }

    throw;
}

ElementaryCondition::~ElementaryCondition() {}
