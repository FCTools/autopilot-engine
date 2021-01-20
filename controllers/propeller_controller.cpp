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
#include <unordered_map>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "spdlog/spdlog.h"

#include "propeller_controller.h"

using namespace std;


string _build_tracker_url(const string base_url, const string period, const string campaign_id)
{
    return base_url + period + "&camp_id=" + campaign_id;
}


PropellerController::PropellerController() : BaseController() {}

unordered_map<string, double> PropellerController::get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                                                     const size_t period, const string api_key) const
{
    unordered_map<string, double> result = {{"ROI", 0}, {"CR", 0}, {"EPC", 0}, {"CPC", 0}};
    list<string> headers = {"Content-Type: application/json", "Accept: application/json"};
    const string url = _build_tracker_url(this->tracker_requests_url, to_string(period), to_string(campaign_tracker_id));
    float cost, revenue, clicks;
    int leads;

    spdlog::info("Start request: " + url);

    string campaign_info = this->make_request(headers, string(), url, "GET");

    if (campaign_info.size() == 0)
    {
        spdlog::error("Error while trying to make request (or empty result)");
    }

    try
    {
        cost = stof(this->get_field_value("cost", campaign_info));
        revenue = stof(this->get_field_value("revenue", campaign_info));
        clicks = stof(this->get_field_value("clicks", campaign_info));
        leads = stoi(this->get_field_value("leads", campaign_info));
    }
    catch (const invalid_argument& exc)
    {
        spdlog::error(exc.what());
        throw IncorrectResponse();
    }

    result["cost"] = cost;
    result["revenue"] = revenue;
    result["clicks"] = clicks;
    result["leads"] = leads;
    result["profit"] = revenue - cost;

    if (cost > 0)
    {
        result["ROI"] = (revenue / cost - 1) * 100;
    }
    
    if (clicks > 0)
    {
        result["CR"] = 100 * leads / clicks;
        result["EPC"] = revenue / clicks;
        result["CPC"] = cost / clicks;
    }
    
    return result;
}

string PropellerController::get_field_value(const string field_name, const string data) const
{
    string pattern = "\"" + field_name + "\":\"";
    if (data.find(pattern) == string::npos)
    {
        throw IncorrectResponse();
    }

    size_t start_pos = data.find(pattern) + pattern.length();
    size_t end_pos = data.find("\"", start_pos);
    string str = data.substr(start_pos, end_pos - start_pos);

    return str;
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
