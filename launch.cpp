#include <iostream>
#include "helpers/conditions_parser.h"

using namespace std;

int main(int argc, char* argv[])
{
    // get bot condition from database here
    string condition = "((revenue > 10) & (cost > 5))";

    ConditionsParser parser;

    Expression* parsed_condition = parser.parse(condition);
    cout << "hello world" << endl;

}
