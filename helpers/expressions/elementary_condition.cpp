// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <unordered_map>

#include "elementary_condition.h"

ElementaryCondition::ElementaryCondition(): Expression() {}

ElementaryCondition::ElementaryCondition(string source): Expression()
{
    source = source.substr(1, source.length() - 2);
    
    if (source.find('=') != string::npos && (source.find("<") != string::npos || 
                                             source.find(">") != string::npos))
    {
        size_t relation_index = source.find('=');
        this->operation = source.substr(relation_index - 1, 2);
        this->param = source.substr(0, relation_index - 1);
        this->value = stod(source.substr(relation_index + 1, source.length() - relation_index - 1));

    }
    else
    {
        size_t relation_index = source.find('<');
        if (relation_index == string::npos)
        {
            relation_index = source.find('>');
        }

        this->operation = source.substr(relation_index, 1);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 1, source.length() - relation_index - 1));
    }

    cout << this->operation << endl;
    cout << this->param << endl;
    cout << this->value << endl;
}


bool ElementaryCondition::is_true(unordered_map<string, double>& campaign_info)
{
    double fact_value = campaign_info[this->param];

    if (this->operation == "<")
    {
        return fact_value < this->value;
    }
    else if (this->operation == "<=")
    {
        return fact_value <= this->value;
    }
    else if (this->operation == ">")
    {
        return fact_value > this->value;
    }
    else if (this->operation == ">=")
    {
        return fact_value >= this->value;
    }

    throw;
}

ElementaryCondition::~ElementaryCondition() {}
