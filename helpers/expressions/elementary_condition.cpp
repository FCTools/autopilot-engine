#include <string>
#include "elementary_condition.h"

ElementaryCondition::ElementaryCondition(): Expression() {}

ElementaryCondition::ElementaryCondition(string source): Expression()
{
    source = source.substr(1, source.length() - 2);
    
    if (source.find('=') != string::npos)
    {
        size_t relation_index = source.find('=');
        this->operation = source.substr(relation_index - 1, 2);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 2, source.length() - relation_index - 2));
    }
    else
    {
        size_t relation_index = source.find('<');
        if (relation_index == string::npos)
        {
            relation_index = source.find('>');
        }

        this->operation = source.substr(relation_index, 1);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 1, source.length() - relation_index - 1));
    }
    
}
