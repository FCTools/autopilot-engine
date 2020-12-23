#include <string>
#include <sstream>
#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/OptionBase.hpp>
#include <curlpp/Easy.hpp>

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
    string post_fields = "{\"group_by\": \"banner_id\", \"day_from\": \"2020-01-01 00:00:00\", \"day_to\": \"2020-02-01 23:59:59\", \"tz\": \"+0300\", \"campaign_id\": " + campaign_id_str + ",\"geo\": [],\"dept\": [\"nativeads\"]}";

    cURLpp::Easy request;
    std::ostringstream response;

    request.setOpt(new cURLpp::options::HttpHeader(headers));
    request.setOpt(new cURLpp::options::Url(this->requests_url));
    request.setOpt(new cURLpp::options::PostFields(post_fields));
    request.setOpt(new cURLpp::options::PostFieldSize(post_fields.length()));
    request.setOpt(new cURLpp::options::WriteStream(&response));
    request.setOpt(new cURLpp::Options::Verbose(true));

    request.perform();

    return 0;
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
