#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <iostream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "propeller_controller.h"

using namespace std;


PropellerController::PropellerController() : BaseController()
{
    this->api_token = this->config["PROPELLER_API_KEY"];
}

double PropellerController::get_campaign_cost(size_t campaign_id)
{
    list<string> headers;

    headers.push_back("Content-Type: application/json");
    headers.push_back("Authorization: Bearer " + this->api_token);
    headers.push_back("Accept: application/json");

    string campaign_id_str = to_string(campaign_id);
    string post_fields = "{\"group_by\": \"banner_id\","
                         "\"day_from\": \"2020-01-01 00:00:00\","
                         "\"day_to\": \"2020-03-01 23:59:59\"," 
                         "\"campaign_id\": [" + campaign_id_str + "],\"geo\": [],\"dept\": []}";

    cURLpp::Easy request;
    std::ostringstream response;

    request.setOpt<cURLpp::options::HttpHeader>(headers);
    request.setOpt<cURLpp::options::Url>(this->requests_url);
    request.setOpt<cURLpp::options::PostFields>(post_fields);
    request.setOpt<cURLpp::options::PostFieldSize>(post_fields.length());
    request.setOpt<cURLpp::options::WriteStream>(&response);
    request.setOpt<cURLpp::Options::Verbose>(true);

    request.perform();
    string content = response.str();
    size_t offset = 0;

    double result = 0;

    while (content.find("\"spent\":", offset) != string::npos)
    {
        size_t cost_start_pos = content.find("\"spent\":", offset) + strlen("\"spent\":");
        size_t cost_end_pos = content.find("\"", cost_start_pos);

        string cost_str = content.substr(cost_start_pos, cost_end_pos - cost_start_pos - 1);
        result += stod(cost_str);
        offset = cost_end_pos;
    }

    return result;
}

double PropellerController::get_campaign_profit(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_revenue(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_cr(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_roi(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_cpa(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_cpc(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_cpm(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_clicks(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_epc(size_t campaign_id)
{
    return 0;
}

double PropellerController::get_campaign_leads(size_t campaign_id)
{
    return 0;
}
