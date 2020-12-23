#include <iostream>

#include "helpers/conditions_parser.h"
#include "controllers/base_controller.h"
#include "controllers/propeller_controller.h"

using namespace std;

int main(int argc, char* argv[])
{
    // get bot condition from database here
    string condition = "((revenue > 10) & (cost > 5))";

    ConditionsParser parser;

    Expression* parsed_condition = parser.parse(condition);
    
    // choose right client here
    BaseController* controller;
    controller = new PropellerController();

    controller->get_campaign_cost(3141415);

    delete parsed_condition;
    delete controller;

    return EXIT_SUCCESS;
}
