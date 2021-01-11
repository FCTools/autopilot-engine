// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <iostream>
#include <ctime>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "propeller_controller.h"
#include "config_reader.h"

using namespace std;


PropellerController::PropellerController() : BaseController()
{
    auto config = read_config("config.env");
}

string PropellerController::get_campaign_info(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    string campaign_id_str = to_string(campaign_id);
    string post_fields = "{\"group_by\": \"campaign_id\","
                         "\"day_from\": \"" + start_date + "\","
                         "\"day_to\": \"" + end_date + "\"," 
                         "\"campaign_id\": [" + campaign_id_str + "],\"geo\": [],\"dept\": []}";
    list<string> headers = {"Content-Type: application/json", "Authorization: Bearer " + api_key,
                             "Accept: application/json"};

    return this->make_request(headers, post_fields, this->requests_url);
}

double PropellerController::get_campaign_cost(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    string response = this->get_campaign_info(campaign_id, start_date, end_date, api_key);

    size_t cost_start_pos = response.find("\"spent\":") + strlen("\"spent\":");
    size_t cost_end_pos = response.find("\"", cost_start_pos);
    string cost_str = response.substr(cost_start_pos, cost_end_pos - cost_start_pos - 1);

    return stod(cost_str);
}

double PropellerController::get_campaign_profit(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    string response = this->get_campaign_info(campaign_id, start_date, end_date, api_key);

    size_t profit_start_pos = response.find("\"profit\":") + strlen("\"profit\":");
    size_t profit_end_pos = response.find("\"", profit_start_pos);
    string profit_str = response.substr(profit_start_pos, profit_end_pos - profit_start_pos - 1);

    return stod(profit_str);
}

double PropellerController::get_campaign_revenue(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_cr(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_roi(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_cpa(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_cpc(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_cpm(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_clicks(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    string response = this->get_campaign_info(campaign_id, start_date, end_date, api_key);

    size_t clicks_start_pos = response.find("\"clicks\":") + strlen("\"clicks\":");
    size_t clicks_end_pos = response.find("\"", clicks_start_pos);
    string clicks_str = response.substr(clicks_start_pos, clicks_end_pos - clicks_start_pos - 1);

    return stod(clicks_str);
}

double PropellerController::get_campaign_epc(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

double PropellerController::get_campaign_leads(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const
{
    return 0;
}

string PropellerController::get_now() const
{
    time_t rawtime;
    tm* timeinfo;
    char buffer[256];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    time_t epoch = mktime(timeinfo);
    epoch -= 7 * 60 * 60;  // make time utc

    timeinfo = localtime(&epoch);

    strftime(buffer, 256, "%Y-%m-%d %T", timeinfo);

    return string(buffer);
}

string PropellerController::get_past_time(const size_t seconds) const
{
    time_t rawtime;
    tm* timeinfo;
    char buffer[256];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    time_t epoch = mktime(timeinfo);
    epoch -= (7 * 60 * 60 + (long)seconds);  // make time utc and substract seconds value

    timeinfo = localtime(&epoch);

    strftime(buffer, 256, "%Y-%m-%d %T", timeinfo);

    return string(buffer);
}

PropellerController::~PropellerController() {}
