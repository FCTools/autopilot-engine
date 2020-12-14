#include "conditions_parser.h"
#include "expressions/elementary_condition.h"


Expression* ConditionsParser::parse(string source)
{
    if (source.find("&") == string::npos && source.find("|") == string::npos) // elementary condition checking
    {
        return new ElementaryCondition(source);
    }
    
    source = source.substr(1, source.length() - 2);

    
}
