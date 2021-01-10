// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;

class BaseController
{

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

    virtual string get_now() = 0;

    virtual string get_past_time(size_t seconds) = 0;

    string make_request(list<string> headers, string body, string url);

    virtual ~BaseController();
};
