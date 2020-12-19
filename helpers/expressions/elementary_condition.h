#pragma once
#include <string>

#include "expression.h"
using namespace std;


class ElementaryCondition: public Expression
{
    string param;
    double value;
    string operation;

public:
    ElementaryCondition();

    ElementaryCondition(string source);

    double get_fact_value(string source, size_t campaign_id);

    bool is_true(string traffic_source, size_t campaign_id);
};
