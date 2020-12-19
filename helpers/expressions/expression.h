#pragma once

class Expression
{
    bool is_binary;

public:
    Expression();

    virtual bool is_true() = 0;
};
