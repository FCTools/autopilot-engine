#pragma once

#include "expression.h"

class BinaryOperation: public Expression
{
    char operation; // & or |
    Expression* left;
    Expression* right;

public:
    BinaryOperation(Expression* l, Expression* r, char operation);

    virtual bool is_true();

    ~BinaryOperation();
};
