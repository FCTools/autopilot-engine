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

struct IncorrectResponse : public std::exception
{
	const char * what () const throw ()
    {
    	return "Can't find required parameter in http-response.";
    }
}

class BaseController
{

public:
    BaseController();

    virtual unordered_map<string, double> get_campaign_info(const size_t campaign_tracker_id, const string campaign_source_id, const size_t period, 
                                                            const string api_key) const = 0;

    virtual string get_now() const = 0;

    virtual string get_past_time(const size_t seconds) const = 0;

    string make_request(const list<string> headers, const string body, const string url, 
                        const string type = "POST") const;

    virtual ~BaseController();
};
