// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <string>
#include <sstream>
#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "helpers/http.h"

#define DEFAULT_TIMEOUT 30

namespace http {
    const char* IncorrectResponse::what() const throw() {
        return "Can't find required parameter in http-response.";
    }

    const char* RequestError::what() const throw() {
        return "Can't find required parameter in http-response.";
    }

    std::string make_request(const std::list<std::string> headers,
                             const std::string body,
                             const std::string url,
                             const std::string type) {
        cURLpp::Cleanup cleanup;

        cURLpp::Easy request;
        std::ostringstream response;

        request.setOpt<cURLpp::options::HttpHeader>(headers);
        request.setOpt<cURLpp::options::Url>(url);
        request.setOpt<cURLpp::options::PostFields>(body);
        request.setOpt<cURLpp::options::PostFieldSize>(body.length());
        request.setOpt<cURLpp::options::WriteStream>(&response);
        request.setOpt<cURLpp::options::Verbose>(true);
        request.setOpt<cURLpp::options::Timeout>(DEFAULT_TIMEOUT);

        if (type != "POST") {
            request.setOpt<cURLpp::options::CustomRequest>(type);
        }

        try {
            request.perform();
        }
        catch(...) {
            throw RequestError();
        }

        return response.str();
    }

    std::string build_url(const std::string base_url, std::unordered_map<std::string, std::string> params)
    {
        std::string result = base_url + "?";

        for (auto &param : params)
        {
            result += param.first + "=" + param.second + "&";
        }
        result = result.substr(0, result.length() - 1);

        return result;
    }

}  // namespace http
