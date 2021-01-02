#pragma once
#include <string>

#include "base_controller.h"

using namespace std;

class PropellerController: public BaseController
{
    string api_token;
    const string requests_url = "https://ssp-api.propellerads.com/v5/adv/statistics";

public:
    PropellerController();

    virtual double get_campaign_cost(size_t campaign_id);

    virtual double get_campaign_profit(size_t campaign_id);

    virtual double get_campaign_revenue(size_t campaign_id);

    virtual double get_campaign_cr(size_t campaign_id);

    virtual double get_campaign_roi(size_t campaign_id);

    virtual double get_campaign_cpa(size_t campaign_id);

    virtual double get_campaign_cpc(size_t campaign_id);

    virtual double get_campaign_cpm(size_t campaign_id);

    virtual double get_campaign_clicks(size_t campaign_id);

    virtual double get_campaign_epc(size_t campaign_id);

    virtual double get_campaign_leads(size_t campaign_id);
};
