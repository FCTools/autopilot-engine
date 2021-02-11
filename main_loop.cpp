// Copyright © 2020-2021 Filthy Claws Tools - All Rights Reserved
//
// This file is part of autopilot.autopilot-engine.
//
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Author: German Yakimov <german13yakimov@gmail.com>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <sstream>
#include <set>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "redis_client.h"
#include "database_client.h"
#include "conditions.h"
#include "conditions_parser.h"
#include "base_controller.h"
#include "propeller_controller.h"
#include "http.h"
#include "main_loop.h"

using namespace std;

const size_t checking_timeout = (size_t)stoi(string(getenv("CHECKING_TIMEOUT")));
mutex storage_mutex, actions_mutex;
condition_variable cond_var;


// uuid generating logic
namespace uuid 
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 15);
    static uniform_int_distribution<> dis2(8, 11);

    string generate_uuid_v4() 
    {
        stringstream ss;
        ss << hex;

        for (size_t _ = 0; _ < 8; _++)
        {
            ss << dis(gen);
        }
        ss << "-";

        for (size_t _ = 0; _ < 4; _++) 
        {
            ss << dis(gen);
        }
        ss << "-4";

        for (size_t _ = 0; _ < 3; _++) 
        {
            ss << dis(gen);
        }
        ss << "-" << dis2(gen);

        for (size_t _ = 0; _ < 3; _++) 
        {
            ss << dis(gen);
        }
        ss << "-";

        for (size_t _ = 0; _ < 12; _++)
        {
            ss << dis(gen);
        }

        return ss.str();
    }
}

set<string> _split(string source, char delimiter)
{
    if (source.find(delimiter) == string::npos)
	{
		return { source };
	}

	size_t current = 0, next;
	set<string> tokens;

	while (source.find(delimiter, current) != string::npos)
	{
		next = source.find(delimiter, current);
		tokens.insert(source.substr(current, next - current));
		current = next + 1;
	}
	tokens.insert(source.substr(current, source.length() - current));

	return tokens;
}

void _extend_storage(vector<string>& storage, vector<string>& new_tasks)
{
    lock_guard<mutex> lock_(storage_mutex);
    storage.reserve(storage.size() + distance(new_tasks.begin(), new_tasks.end()));
    storage.insert(storage.end(), new_tasks.begin(), new_tasks.end());
}

// check for new tasks in storage redis
void _start_queue_updating_process(vector<string>& tasks)
{
    RedisClient redis;

    while (true)
    {
        vector<string> updates = redis.get_updates();

        if (!updates.empty())
        {
            _extend_storage(tasks, updates);
            cond_var.notify_all();
        }

        this_thread::sleep_for(chrono::seconds(checking_timeout));
    }

}

void _put_action(RedisClient& redis, const string data)
{
    string task_id = uuid::generate_uuid_v4();

    lock_guard<mutex> lock_(actions_mutex);
    redis.put_action(task_id, data);
}

BaseController* _get_controller(string ts)
{
    if (ts == "Propeller Ads")
    {
        return new PropellerController();
    }
    else
    {
        spdlog::get("file_logger")->error("Can't choose controller for traffic source " + ts);
        return nullptr;
    }
}

void _check_campaign(const size_t bot_id, unordered_map<string, string>& bot_info)
{
    RedisClient redis;

    string condition = bot_info["condition"];
    size_t period = (size_t)stoi(bot_info["period"]);
    string api_key = bot_info["api_key"];
    string ts = bot_info["ts"];
    string action = bot_info["action"];

    // get bot campaigns from database
    vector<size_t> campaigns_ids = database::get_bot_campaigns(bot_id);

    ConditionsParser parser;

    spdlog::get("file_logger")->info("Start parsing for this condition: " + condition);
    BaseCondition* parsed_condition = parser.parse_condition(condition);
    spdlog::get("file_logger")->info("Condition " + condition + " was successfully parsed");

    string ts_name = database::get_ts_name((size_t)stoi(ts));
    BaseController* controller = _get_controller(ts_name);
    if (!controller)
    {
        return;
    }
    spdlog::get("file_logger")->info("Select controller for " + ts_name);
    
    // check bot campaigns
    for (size_t campaign_id: campaigns_ids)
    {
        auto ids = database::get_campaign_ids(campaign_id);
        size_t tracker_id = ids.first;
        string source_id = ids.second;

        unordered_map<string, double> campaign_info;

        campaign_info = controller->get_campaign_info(tracker_id, source_id, period, api_key);

        if (campaign_info.size() == 0)
        {
            spdlog::get("file_logger")->error("Can't get campaign info. Skip campaign: " + to_string(campaign_id));
            continue;
        }

        if (parsed_condition->is_true(campaign_info))
        {
            string data = "{\"campaign_id\": " + source_id + ", \"action\": "
            + action + ", \"ts\": \"" + ts_name + "\", \"api_key\": \"" + api_key + "\"}";

            spdlog::get("file_logger")->info("Condition is true. Bot id: " +to_string(bot_id));
            _put_action(redis, data);
        }
    }

    delete controller;
}

void _check_zones(const size_t bot_id, unordered_map<string, string>& bot_info)
{
    RedisClient redis;

    string condition = bot_info["condition"];
    size_t period = (size_t)stoi(bot_info["period"]);
    string api_key = bot_info["api_key"];
    string ts = bot_info["ts"];
    string action = bot_info["action"];

    // get bot campaigns from database
    vector<size_t> campaigns_ids = database::get_bot_campaigns(bot_id);
    set<string> ignored_zones = _split(bot_info["ignored_zones"], '\n');

    ConditionsParser parser;

    spdlog::get("file_logger")->info("Start parsing for this condition: " + condition);
    BaseCondition* parsed_condition = parser.parse_condition(condition);
    spdlog::get("file_logger")->info("Condition " + condition + " was successfully parsed");

    string ts_name = database::get_ts_name((size_t)stoi(ts));
    BaseController* controller = _get_controller(ts_name);
    if (!controller)
    {
        return;
    }
    spdlog::get("file_logger")->info("Select controller for " + ts_name);

    vector<string> zones_to_act;

    for (size_t campaign_id: campaigns_ids)
    {
        auto ids = database::get_campaign_ids(campaign_id);
        size_t tracker_id = ids.first;
        string source_id = ids.second;

        auto zones_info = controller->get_zones_info(tracker_id, source_id, period, api_key, ref(ignored_zones));

        if (zones_info.size() == 0)
        {
            spdlog::get("file_logger")->error("Can't get zones info for campaign " + to_string(tracker_id) + ". Skip.");
            continue;
        }

        for (auto& zone: zones_info)
        {
            if (parsed_condition->is_true(zone.second))
            {
                zones_to_act.push_back(zone.first);
            }
        }

        string zones_to_act_string = "[";
        for (auto zone: zones_to_act)
        {
            zones_to_act_string += "\"" + zone + "\",";
        }
        if (zones_to_act_string != "[")
        {
            zones_to_act_string = zones_to_act_string.substr(0, zones_to_act_string.length() - 1);
        }
        else
        {
            spdlog::get("file_logger")->info("Condition is true for 0 zones. Bot id: " + to_string(bot_id));
            continue;
        }
        zones_to_act_string += "]";

        string data = "{\"campaign_id\": " + source_id + ", \"action\": "
                + action + ", \"ts\": \"" + ts_name + "\", \"zones\": " + 
                zones_to_act_string +", \"api_key\": \"" + api_key + "\"}";

        spdlog::get("file_logger")->info("Condition is true for " + to_string(zones_to_act.size()) + 
                                         " zones. Bot id: " + to_string(bot_id));
        _put_action(redis, data);
    }

    delete controller;
}

// processing bot
void _process_task(const string bot_id_str)
{
    const size_t bot_id = (size_t)stoi(bot_id_str);
    auto bot_info = database::get_bot_info(bot_id);
    size_t action = (size_t)stoi(bot_info["action"]);

    switch (action)
    {
        case START_CAMPAIGN:
        case STOP_CAMPAIGN:
            _check_campaign(bot_id, ref(bot_info));
            break;
        case INCLUDE_ZONE:
        case EXCLUDE_ZONE:
            _check_zones(bot_id, ref(bot_info));
            break;
        default:
            spdlog::get("file_logger")->error("Unknown action: " + to_string(action));
            break;
    }
}

void _worker_main_function(vector<string>& storage)
{
    spdlog::get("file_logger")->info("Initialize worker.");

    while (true)
    {
        unique_lock<mutex> unique_storage_mutex(storage_mutex);
		cond_var.wait(unique_storage_mutex, [&storage]{return !storage.empty(); });
        
        string bot_id = *storage.begin();
        storage.erase(storage.begin());
        unique_storage_mutex.unlock();

        spdlog::get("file_logger")->info("Get new task. Bot id: " + bot_id);
        _process_task(bot_id);
        spdlog::get("file_logger")->info("Finish task. Bot id: " + bot_id);
    }
}

void start(const size_t workers_num)
{
    vector<string> tasks;
    vector<thread> workers_pool;

    spdlog::get("file_logger")->info("Create all resources (mutexes, containers). Start to creating workers.");

    for (size_t _ = 0; _ < workers_num; _++)
    {
        workers_pool.emplace_back(_worker_main_function, ref(tasks));
        (*(workers_pool.end() - 1)).detach();
    }

    spdlog::get("file_logger")->info("Create and detach workers. Start storage updating.");

    _start_queue_updating_process(ref(tasks));
}
