#pragma once

#include <string>
#include <list>

using namespace std;

namespace http
{
    struct IncorrectResponse : public std::exception
    {
        const char * what () const throw ();
    };

    struct RequestError : public std::exception
    {
        const char * what () const throw ();
    };

    string make_request(const list<string> headers, const string body, const string url, 
                                    const string type);
}
