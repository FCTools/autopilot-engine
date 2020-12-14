#pragma once
#include <string>

#include "expressions/expression.h"
using namespace std;


class ConditionsParser
{
public:
    ConditionsParser();

    Expression* parse(string source);

};

