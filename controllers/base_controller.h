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

    virtual double get_campaign_cost(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_profit(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_revenue(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_cr(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_roi(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_cpa(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_cpc(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_cpm(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_clicks(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_epc(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual double get_campaign_leads(const size_t campaign_id, const string start_date, const string end_date, const string api_key) const = 0;

    virtual string get_now() const = 0;

    virtual string get_past_time(const size_t seconds) const = 0;

    string make_request(const list<string> headers, const string body, const string url) const;

    virtual ~BaseController();
};
