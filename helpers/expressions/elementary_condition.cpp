// Copyright © 2020-2021 FC Tools. 
// All rights reserved.
// Author: German Yakimov

#include <string>

#include "elementary_condition.h"
#include "base_controller.h"
#include "propeller_controller.h"

ElementaryCondition::ElementaryCondition(): Expression() {}

ElementaryCondition::ElementaryCondition(string source): Expression()
{
    source = source.substr(1, source.length() - 2);
    
    if (source.find('=') != string::npos)
    {
        size_t relation_index = source.find('=');
        this->operation = source.substr(relation_index - 1, 2);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 2, source.length() - relation_index - 2));
    }
    else
    {
        size_t relation_index = source.find('<');
        if (relation_index == string::npos)
        {
            relation_index = source.find('>');
        }

        this->operation = source.substr(relation_index, 1);
        this->param = source.substr(0, relation_index);
        this->value = stod(source.substr(relation_index + 1, source.length() - relation_index - 1));
    }
    
}

double ElementaryCondition::get_fact_value(BaseController* controller, size_t campaign_id, string start_date, string end_date, string api_key)
{
    if (this->param == "revenue")
    {
        return controller->get_campaign_revenue(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "cost")
    {
        return controller->get_campaign_cost(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "profit")
    {
        return controller->get_campaign_profit(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "clicks")
    {
        return controller->get_campaign_clicks(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "CPC")
    {
        return controller->get_campaign_cpc(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "ROI")
    {
        return controller->get_campaign_roi(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "CR")
    {
        return controller->get_campaign_cr(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "EPC")
    {
        return controller->get_campaign_epc(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "leads")
    {
        return controller->get_campaign_leads(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "cpa")
    {
        return controller->get_campaign_cpa(campaign_id, start_date, end_date, api_key);
    }
    else if (this->param == "approve_%")
    {
        
    }

    throw;
}

bool ElementaryCondition::is_true(BaseController* controller, size_t campaign_id, string start_date, string end_date, string api_key)
{
    double fact_value = this->get_fact_value(controller, campaign_id, start_date, end_date, api_key);

    if (this->operation == "<")
    {
        return fact_value < this->value;
    }
    else if (this->operation == "<=")
    {
        return fact_value <= this->value;
    }
    else if (this->operation == ">")
    {
        return fact_value > this->value;
    }
    else if (this->operation == ">=")
    {
        return fact_value >= this->value;
    }

    throw;
}

ElementaryCondition::~ElementaryCondition() {}
