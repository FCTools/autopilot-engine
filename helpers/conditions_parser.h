#pragma once
#include <string>

#include "expressions/expression.h"
using namespace std;


class ConditionsParser
{
    Expression* build(string source);

public:
    ConditionsParser();

    Expression* parse(string source);

};

