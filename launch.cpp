// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>


#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <locale>

#include "conditions_parser.h"
#include "base_controller.h"
#include "propeller_controller.h"
#include "database_client.h"
#include "redis_client.h"

using namespace std;

int main(int argc, char** argv)
{
    //get bot id from args
    if (argc != 2)
    {
        throw invalid_argument("Invalid arguments number given.");
    }
    size_t bot_id = (size_t)stoi(argv[1]);

    // get bot condition and period from database

    auto database_client = new DatabaseClient();
    string condition = database_client->get_bot_condition(bot_id);
    size_t period = database_client->get_bot_period(bot_id);

    // get bot campaigns here
    vector<size_t> campaigns_ids = database_client->get_bot_campaigns(bot_id);

    auto parser = new ConditionsParser();
    Expression* parsed_condition = parser->parse(condition);
    
    // choose right client here
    BaseController* controller;
    controller = new PropellerController();

    // test
    // double cost = controller->get_campaign_cost(3141415, "2020-01-01 00:00:00", "2020-03-01 23:59:59");
    // double profit = controller->get_campaign_profit(3141415, "2020-01-01 00:00:00", "2020-03-01 23:59:59");
    // double clicks = controller->get_campaign_clicks(3141415, "2020-01-01 00:00:00", "2020-03-01 23:59:59");

    auto redis = new RedisClient();

    size_t action = database_client->get_bot_action(bot_id);
    cout << action << endl;

    string now_dt = controller->get_now();
    string from_dt = controller->get_past_time(period);
    cout << now_dt << endl;
    cout << from_dt << endl;

    string campaigns_status = "{";

    // for (auto campaign: campaigns_ids)
    // {
        // if (parsed_condition->is_true(campaign))
    // }

    redis->put("test_key", "test_value");
    redis->get_updates();

    delete parsed_condition;
    delete controller;
    delete database_client;
    delete parser;
    delete redis;

    return EXIT_SUCCESS;
}
