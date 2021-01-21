#include <string>
#include <sstream>
#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "http.h"

using namespace std;

namespace http
{
    const char* IncorrectResponse::what () const throw ()
    {
        return "Can't find required parameter in http-response.";
    }

    const char* RequestError::what () const throw ()
    {
        return "Can't find required parameter in http-response.";
    }

    string make_request(const list<string> headers, const string body, const string url, 
                                    const string type)
    {
        cURLpp::Cleanup cleanup;

        cURLpp::Easy request;
        ostringstream response;

        request.setOpt<cURLpp::options::HttpHeader>(headers);
        request.setOpt<cURLpp::options::Url>(url);
        request.setOpt<cURLpp::options::PostFields>(body);
        request.setOpt<cURLpp::options::PostFieldSize>(body.length());
        request.setOpt<cURLpp::options::WriteStream>(&response);
        request.setOpt<cURLpp::Options::Verbose>(true);

        if (type != "POST")
        {
            request.setOpt<cURLpp::options::CustomRequest>(type);
        }

        try
        {
            request.perform();
        }
        catch(...)
        {
            throw RequestError();
        }

        return response.str();
    }
}
