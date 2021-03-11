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

namespace http {
struct IncorrectResponse : public std::exception {
    const char* what() const throw();
};

struct RequestError : public std::exception {
    const char* what() const throw();
};

    std::string make_request(const std::list<std::string> headers,
                             const std::string body,
                             const std::string url,
                             const std::string type);
}  // namespace http
