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


PropellerController::PropellerController() : BaseController() {}

// where is leads?
unordered_map<string, double> PropellerController::get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                                                     const size_t period, const string api_key) const
{
    unordered_map<string, double> result;
    // unordered_map<string, string> params_aliases = {{"cost", "spent"}, {"profit", "profit"}, {"CR", "cr"}, {"clicks", "clicks"}, 
    //                                                 {"ROI", "roi"}, {"CPC", "cpc"}, {"CPA", "cpa"}, {"CPM", "cpm"}, {"leads", "conversions"}};

    string post_fields = "{}";
    list<string> headers = {"Content-Type: application/json", "Accept: application/json"};

    string url = this->tracker_requests_url + to_string(period);
    spdlog::info("Start request: " + url);
    string data = this->make_request(headers, post_fields, url);

    size_t start_pos = data.find(to_string(campaign_tracker_id));

    if (start_pos ==  string::npos)
    {
        spdlog::error("Can't find info in tracker response about campaign " + to_string(campaign_tracker_id));
        spdlog::error("Tracker response: " + data);
        return result;
    }

    // string campaign_id_str = to_string(campaign_id);
    // string post_fields = "{\"group_by\": \"campaign_id\"," 
    //                      "\"day_from\": \"" + start_date + "\","
    //                      "\"day_to\": \"" + end_date + "\"," 
    //                      "\"campaign_id\": [" + campaign_id_str + "],\"geo\": [],\"dept\": []}";
    // list<string> headers = {"Content-Type: application/json", "Authorization: Bearer " + api_key,
    //                          "Accept: application/json"};

    // string data = this->make_request(headers, post_fields, this->tracker_requests_url);
    // cout << data << endl;

    // cout << post_fields << endl;

    // for (auto param: params_aliases)
    // {
    //     result.insert({param.first, this->get_field_value(param.second, data)});
    // }

    // result.insert({"revenue", result["cost"] + result["profit"]});

    // if (result["clicks"] != 0)
    // {
    //     result.insert({"EPC", result["revenue"] / result["clicks"]});
    // }
    // else
    // {
    //     result.insert({"EPC", 0});
    // }
    

    return result;
}

double PropellerController::get_field_value(const string field_name, const string data) const
{
    string pattern = "\"" + field_name + "\":";
    size_t start_pos = data.find(pattern) + pattern.length();
    size_t end_pos = data.find("\"", start_pos);
    string str = data.substr(start_pos, end_pos - start_pos - 1);

    return stod(str);
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
