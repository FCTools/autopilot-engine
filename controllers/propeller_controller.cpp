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
#include <vector>
#include <iostream>
#include <ctime>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "propeller_controller.h"
#include "http.h"

using namespace std;

// insert period and campaign_id as query parameters
string _build_request_url(const string base_url, const string period, const string campaign_id)
{
    return base_url + period + "&camp_id=" + campaign_id;
}


PropellerController::PropellerController() : BaseController() {}

unordered_map<string, double> PropellerController::get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, 
                                                                     const size_t period, const string api_key) const
{
    unordered_map<string, double> result = {{"ROI", 0.}, {"CR", 0.}, {"EPC", 0.}, {"CPC", 0.}};
    list<string> headers = {"Content-Type: application/json", "Accept: application/json"};
    const string url = _build_request_url(this->tracker_requests_url, to_string(period), to_string(campaign_tracker_id));
    float cost = 0., revenue = 0., clicks = 0.;
    int leads = 0;

    spdlog::get("file_logger")->info("Perform request: " + url);

    string campaign_info = http::make_request(headers, string(), url, "GET");

    if (campaign_info.size() == 0)
    {
        spdlog::get("file_logger")->error("Error while trying to make request (or empty result)");
        throw http::IncorrectResponse();
    }

    try
    {
        vector<string> costs_by_paths = this->get_field_values("cost", campaign_info);
        vector<string> revenues_by_paths = this->get_field_values("cost", campaign_info);
        vector<string> clicks_by_paths = this->get_field_values("cost", campaign_info);
        vector<string> leads_by_paths = this->get_field_values("cost", campaign_info);

        for (string cost_path: costs_by_paths)
        {
            cost += stof(cost_path);
        }
        
        for (string revenue_path: revenues_by_paths)
        {
            revenue += stof(revenue_path);
        }

        for (string clicks_path: clicks_by_paths)
        {
            clicks += stof(clicks_path);
        }

        for (string leads_path: leads_by_paths)
        {
            leads += stoi(leads_path);
        }
    }
    catch (const invalid_argument& exc)
    {
        spdlog::get("file_logger")->error(exc.what());
        throw http::IncorrectResponse();
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

vector<string> PropellerController::get_field_values(const string field_name, const string data) const
{
    vector<string> result;

    string pattern = "\"" + field_name + "\":\"";
    if (data.find(pattern) == string::npos)
    {
        throw http::IncorrectResponse();
    }

    size_t start_pos = 0, end_pos;

    while (data.find(pattern, start_pos) != string::npos)
    {
        start_pos = data.find(pattern, start_pos) + pattern.length();
        size_t end_pos = data.find("\"", start_pos);
        string str = data.substr(start_pos, end_pos - start_pos);

        result.push_back(str);
    }

    return result;
}

PropellerController::~PropellerController() {}
