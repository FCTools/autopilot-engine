#pragma once
#include <string>

#include "expressions/expression.h"
using namespace std;


class ConditionsParser
{

    ConditionsParser();

public:

    Expression* parse(string source);

};

