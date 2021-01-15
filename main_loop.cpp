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
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "redis_client.h"
#include "database_client.h"
#include "expression.h"
#include "conditions_parser.h"
#include "base_controller.h"
#include "propeller_controller.h"

using namespace std;

// uuid generating logic
namespace uuid {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 15);
    static uniform_int_distribution<> dis2(8, 11);

    string generate_uuid_v4() {
        stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }
}


void _extend_storage(mutex& storage_mutex, vector<string>& storage, vector<string>& new_tasks)
{
    lock_guard<mutex> lock_(storage_mutex);
    storage.reserve(storage.size() + distance(new_tasks.begin(), new_tasks.end()));
    storage.insert(storage.end(), new_tasks.begin(), new_tasks.end());
}

void _start_queue_updating_process(mutex& storage_mutex, vector<string>& tasks, condition_variable& cond_var)
{
    RedisClient redis;

    while (true)
    {
        vector<string> updates = redis.get_updates();

        if (!updates.empty())
        {
            _extend_storage(storage_mutex, tasks, updates);
            cond_var.notify_all();
        }

        this_thread::sleep_for(chrono::seconds(10));
    }

}

void _put_action(RedisClient& redis, string data, mutex& actions_mutex)
{
    string task_id = uuid::generate_uuid_v4();

    lock_guard<mutex> lock_(actions_mutex);
    redis.put_action(task_id, data);
}

void _process_task(string bot_id_str, mutex& actions_mutex)
{
    DatabaseClient database;
    RedisClient redis;
    size_t bot_id = (size_t)stoi(bot_id_str);

    string condition = database.get_bot_condition(bot_id);
    size_t period = database.get_bot_period(bot_id);
    size_t action = database.get_bot_action(bot_id);
    string api_key = database.get_bot_api_key(bot_id);

    // get bot campaigns here
    vector<size_t> campaigns_ids = database.get_bot_campaigns(bot_id);

    ConditionsParser parser;
    Expression* parsed_condition = parser.parse(condition);

    BaseController* controller = nullptr;
    string ts = database.get_bot_traffic_source(bot_id);

    if (ts == "Propeller Ads")
    {
        controller = new PropellerController();
    }

    if (!controller)
    {
        spdlog::error("Can't choose controller for traffic source " + ts);
        throw;  // throw something
    }

    // string now_dt = controller->get_now();
    // string from_dt = controller->get_past_time(period);

    for (size_t campaign_id: campaigns_ids)
    {
        auto ids = database.get_campaign_ids(campaign_id);
        size_t tracker_id = ids.first;
        string source_id = ids.second;

        unordered_map<string, double> campaign_info = controller->get_campaign_info(tracker_id, source_id, period, api_key);

        if(campaign_info.size() == 0)
        {
            spdlog::error("Skip campaign: " + to_string(campaign_id));
        }

        if (parsed_condition->is_true(campaign_info))
        {
            string data = "{\"campaign_id\": " + to_string(campaign_id) + ", \"action\": "
             + to_string(action) + ", \"ts\": \"" + ts + "\", \"api_key\": \"" + api_key + "\"}";

            _put_action(redis, data, actions_mutex);
        }
    }
}

void _worker_main_function(vector<string>& storage, mutex& storage_mutex, mutex& actions_mutex, condition_variable& cond_var)
{
    spdlog::info("Initialize worker.");

    while (true)
    {
        unique_lock<mutex> unique_storage_mutex(storage_mutex);
		cond_var.wait(unique_storage_mutex, [&storage]{return !storage.empty(); });
        
        string bot_id = *storage.begin();
        storage.erase(storage.begin());
        unique_storage_mutex.unlock();
        spdlog::info("Get new task. Bot id: " + bot_id);
        
        _process_task(bot_id, actions_mutex);
        spdlog::info("Finish task. Bot id: " + bot_id);
    }
}

void launch(size_t workers_num)
{
    mutex storage_mutex;
    mutex actions_mutex;
    vector<string> tasks;
    condition_variable cond_var;
    vector<thread> workers_pool;

    // auto logger = spdlog::get("file_logger");
    // spdlog::get("file_logger")->info("test");

    spdlog::info("Create all resources (mutexes, containers). Start to creating workers.");

    for (size_t worker_id = 0; worker_id < workers_num; worker_id++)
    {
        workers_pool.emplace_back(_worker_main_function, ref(tasks), ref(storage_mutex), ref(actions_mutex), ref(cond_var));
        (*(workers_pool.end() - 1)).detach();
    }

    spdlog::info("Create and detach workers. Start queue storage updating.");

    _start_queue_updating_process(ref(storage_mutex), ref(tasks), ref(cond_var));

}
