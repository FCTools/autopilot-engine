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
#include <unordered_map>

#define DEFAULT_REQUEST_TRIES 5

namespace http
{
const std::string POST = "POST";
const std::string GET = "GET";

struct IncorrectResponse : public std::exception
{
    const char* what() const throw();
};

struct RequestError : public std::exception
{
    const char* what() const throw();
};

    std::string make_request(const std::list<std::string> headers,
                             const std::string body,
                             const std::string url,
                             const std::string type);

    std::string build_url(const std::string base_url, std::unordered_map<std::string, std::string> params);
}  // namespace http
