#include "conditions_parser.h"
#include "expressions/elementary_condition.h"

ConditionsParser::ConditionsParser() {}

Expression* ConditionsParser::parse(string source)
{
    if (source.find("&") == string::npos && source.find("|") == string::npos) // elementary condition checking
    {
        return new ElementaryCondition(source);
    }
    
    source = source.substr(1, source.length() - 2);

    int counter = 0;
    size_t index;

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
    }

    return new ElementaryCondition();
}
