// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#pragma once

#include <string>
#include <list>

using namespace std;

namespace http
{
    struct IncorrectResponse : public exception
    {
        const char* what() const throw();
    };

    struct RequestError : public exception
    {
        const char* what() const throw();
    };

    string make_request(const list<string> headers, const string body, 
                        const string url, const string type);
}
