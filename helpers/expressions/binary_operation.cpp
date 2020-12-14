
#include <iostream>

#include "binary_operation.h"
using namespace std;

BinaryOperation::BinaryOperation(Expression* l, Expression* r, char operation): Expression()
{
    this->left = l;
    this->right = r;

    if (operation != '|' && operation != '&')
    {
        throw invalid_argument("invalid operation: " + to_string(operation));
    }
    this->operation = operation;
}
