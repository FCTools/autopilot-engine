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

    double cost = controller->get_campaign_cost(3141415, "2020-01-01 00:00:00", "2020-03-01 23:59:59");
    double profit = controller->get_campaign_profit(3141415, "2020-01-01 00:00:00", "2020-03-01 23:59:59");
    double clicks = controller->get_campaign_clicks(3141415, "2020-01-01 00:00:00", "2020-03-01 23:59:59");

    delete parsed_condition;
    delete controller;

    return EXIT_SUCCESS;
}
