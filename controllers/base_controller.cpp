// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <fstream>
#include <string>
#include <sstream>
#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "base_controller.h"

using namespace std;

BaseController::BaseController() {}

string BaseController::make_request(list<string> headers, string body, string url)
{
    cURLpp::Easy request;
    ostringstream response;

    request.setOpt<cURLpp::options::HttpHeader>(headers);
    request.setOpt<cURLpp::options::Url>(url);
    request.setOpt<cURLpp::options::PostFields>(body);
    request.setOpt<cURLpp::options::PostFieldSize>(body.length());
    request.setOpt<cURLpp::options::WriteStream>(&response);
    request.setOpt<cURLpp::Options::Verbose>(true);

    request.perform();
    return response.str();
}

BaseController::~BaseController() {}
