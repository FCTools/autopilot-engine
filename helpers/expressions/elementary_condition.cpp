// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#include <string>
#include <unordered_map>

#include "elementary_condition.h"

ElementaryCondition::ElementaryCondition(): Expression() {}

ElementaryCondition::ElementaryCondition(string source): Expression()
{
    source = source.substr(1, source.length() - 2);
    
    if (source.find('=') != string::npos)
    {
        size_t relation_index = source.find('=');
        this->operation = source.substr(relation_index - 1, 2);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 2, source.length() - relation_index - 2));
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
