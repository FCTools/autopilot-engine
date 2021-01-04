// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov


#include <iostream>

#include "conditions_parser.h"
#include "base_controller.h"
#include "propeller_controller.h"
#include "database_client.h"

using namespace std;

int main(int argc, char* argv[])
{
    // get bot condition from database here
    // test
    auto database_client = new DatabaseClient();
    string condition = database_client->get_bot_condition(33);
    // condition = "((revenue > 10) & (cost > 5))";

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
    delete database_client;

    return EXIT_SUCCESS;
}
