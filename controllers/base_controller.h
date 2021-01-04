// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;

class BaseController
{
protected:
    unordered_map<string, string> config;

    void read_config(string filename);

public:
    BaseController();

    virtual double get_campaign_cost(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_profit(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_revenue(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_cr(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_roi(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_cpa(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_cpc(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_cpm(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_clicks(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_epc(size_t campaign_id, string start_date, string end_date) = 0;

    virtual double get_campaign_leads(size_t campaign_id, string start_date, string end_date) = 0;

    string make_request(list<string> headers, string body, string url);
};
