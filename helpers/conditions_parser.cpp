// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <algorithm>
#include <iostream>

#include "conditions_parser.h"
#include "elementary_condition.h"
#include "binary_operation.h"

using namespace std;

ConditionsParser::ConditionsParser() {}

Expression* ConditionsParser::build(string source)
{
    if (source.find("&") == string::npos && source.find("|") == string::npos) // elementary condition checking
    {
        return new ElementaryCondition(source);
    }
    
    source = source.substr(1, source.length() - 2);

    int counter = 0;
    size_t index = 0;

    for (char c: source)
    {
        if (c == '(')
        {
            counter++;
        }
        else if (c == ')')
        {
            counter--;
        }

        if (counter < 0)
        {
            throw;
        }
        if (counter == 0)
        {
            break;
        }
        index++;
    }
    index++;

    char operation = source[index];
    string left = source.substr(0, index);
    string right = source.substr(index + 1, source.length() - index - 1);

    return new BinaryOperation(this->build(left), this->build(right), operation);
}

Expression* ConditionsParser::parse(string source)
{
    source.erase(remove(source.begin(), source.end(), ' '), source.end()); 
    return this->build(source);
}
