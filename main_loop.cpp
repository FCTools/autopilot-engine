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
#include <set>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "data_services/data_services.h"
#include "conditions/conditions.h"
#include "utils/utils.h"
#include "tracker_controllers/tracker_controllers.h"
#include "ts_controllers/ts_controllers.h"
#include "main_loop.h"

const std::size_t checking_timeout = (size_t)std::stoi(std::string(getenv("CHECKING_TIMEOUT")));
std::mutex storage_mutex, actions_mutex;
std::condition_variable cond_var;

void _extend_storage(std::vector<std::string> &storage, std::vector<std::string> &new_tasks)
{
    std::lock_guard<std::mutex> lock_(storage_mutex);
    storage.reserve(storage.size() + distance(new_tasks.begin(), new_tasks.end()));
    storage.insert(storage.end(), new_tasks.begin(), new_tasks.end());
}

// check for new tasks in storage-redis
void queue_updating_process(std::vector<std::string> &tasks)
{
    while (true)
    {
        auto updates = redis::get_updates();

        if (!updates.empty())
        {
            _extend_storage(tasks, updates);
            cond_var.notify_all();
        }

        std::this_thread::sleep_for(std::chrono::seconds(checking_timeout));
    }
}

void _put_action(const std::string data, const std::string key)
{
    std::lock_guard<std::mutex> lock_(actions_mutex);
    redis::put_action(key, data);
}

BaseController *select_controller(std::string ts)
{
    if (ts == PROPELLER_ADS)
    {
        return new PropellerController();
    }
    else if (ts == EVADAV)
    {
        return new EvadavController();
    }
    else if (ts == MGID) {
        return new MgidController();
    }
    else if (ts == KADAM) {
        return new KadamController();
    }
    else if (ts == VIMMY) {
        return new VimmyController();
    }
    else
    {
        spdlog::get("env_logger")->error("Can't choose controller for traffic source " + ts);
        return nullptr;
    }
}

void _check_campaign(const std::size_t bot_id, std::unordered_map<std::string, std::string>& bot_info,
                     std::string task_id)
{
    std::string sys_info = format_log_message(std::to_string(bot_id), task_id);

    auto condition = bot_info["condition"];
    auto period = (size_t)std::stoi(bot_info["period"]);
    auto ts_api_key = bot_info["ts_api_key"];
    auto ts = bot_info["ts"];
    auto action = bot_info["action"];
    auto client_id = bot_info["client_id"];
    auto tracker = bot_info["tracker"];
    auto tracker_requests_url = bot_info["tracker_url"];
    auto tracker_api_key = bot_info["tracker_api_key"];

    // split bot campaigns from database into pairs
    auto campaigns_ids = database::get_bot_campaigns(bot_info["campaigns"]);

    spdlog::get("env_logger")->info(sys_info + "Start condition parsing: " + condition);

    BaseCondition *parsed_condition = conditions_parser::parse_spa_condition(condition);
    spdlog::get("env_logger")->info(sys_info + "Condition " + condition + " was successfully parsed");

    auto ts_name = database::get_ts_name((size_t)std::stoi(ts));
    BaseController *controller = select_controller(ts_name);

    if (!controller)
    {
        return;
    }
    spdlog::get("env_logger")->info(sys_info + "Select controller for " + ts_name);

    // check bot campaigns
    for (auto campaign : campaigns_ids)
    {
        auto tracker_id = campaign.first;
        auto source_id = campaign.second;

        std::unordered_map<std::string, double> campaign_info;

        {  //  getting campaign statistics from tracker
            campaign_info = controller->get_campaign_info(task_id, tracker_id, source_id, period, ts_api_key,
                                                          tracker, tracker_requests_url, tracker_api_key);

            if (campaign_info.size() == 0)
            {
                spdlog::get("actions_logger")->error(sys_info + "Can't get campaign info. Skip campaign: "
                                                     + tracker_id + " | " + source_id);
                continue;
            }
        }

        {  // checking condition for campaign and put handling_result to redis
            if (parsed_condition->is_true(campaign_info))
            {
                auto data = "{\"campaign_id\": " + source_id
                            + ", \"action\": " + action + ", \"ts\": \""
                            + ts_name + "\", \"api_key\": \""
                            + ts_api_key + "\", \"client_id\": \"" + client_id + "\"}";

                    spdlog::get("actions_logger")->info(sys_info + "Condition is true for campaign "
                                                        + tracker_id + " | " + source_id);

                    _put_action(data, task_id);
                }
        }
    }

    delete controller;
    delete parsed_condition;
}

std::string _check_condition_for_zones(zones_data& zones_info, BaseCondition *parsed_condition,
                                       std::vector<std::string>& zones_to_act)
{
    for (auto &zone : zones_info)
    {
        if (parsed_condition->is_true(zone.second))
        {
            if (find(zones_to_act.begin(), zones_to_act.end(), zone.first) == zones_to_act.end())
            {
                zones_to_act.push_back(zone.first);
            }
        }
    }

    std::string zones_to_act_string = "[";
    for (auto zone : zones_to_act)
    {
        zones_to_act_string += "\"" + zone + "\",";
    }

    if (zones_to_act_string != "[")
    {
        zones_to_act_string = zones_to_act_string.substr(0, zones_to_act_string.length() - 1);
        return zones_to_act_string + "]";
    }
    
    return std::string();
}

void _check_zones(const std::size_t bot_id, std::unordered_map<std::string, std::string>& bot_info,
                  std::string task_id)
{
    std::string sys_info = format_log_message(std::to_string(bot_id), task_id);

    auto condition = bot_info["condition"];
    std::size_t period = (std::size_t)stoi(bot_info["period"]);
    auto api_key = bot_info["ts_api_key"];
    auto ts = bot_info["ts"];
    auto action = bot_info["action"];
    auto list_to_add = bot_info["list_to_add"];
    auto client_id = bot_info["client_id"];
    auto tracker = bot_info["tracker"];
    auto tracker_requests_url = bot_info["tracker_url"];
    auto tracker_api_key = bot_info["tracker_api_key"];

    // split bot campaigns from database into pairs
    auto campaigns_ids = database::get_bot_campaigns(bot_info["campaigns"]);
    auto ignored_zones = split(bot_info["ignored_zones"], '\n');

    spdlog::get("env_logger")->info(sys_info + "Start condition parsing: " + condition);
    BaseCondition *parsed_condition = conditions_parser::parse_spa_condition(condition);
    spdlog::get("env_logger")->info(sys_info + "Condition " + condition + " was successfully parsed");

    auto ts_name = database::get_ts_name((size_t)std::stoi(ts));
    BaseController *controller = select_controller(ts_name);
    if (!controller)
    {
        return;
    }
    spdlog::get("env_logger")->info(sys_info + "Select controller for " + ts_name);

    for (auto campaign : campaigns_ids)
    {
        auto tracker_id = campaign.first;
        auto source_id = campaign.second;

        zones_data zones_info;
        std::string zones_to_act_string;
        std::vector<std::string> zones_to_act;

        { // getting zones info from tracker
            spdlog::get("env_logger")->debug(sys_info + "Bot id: " + std::to_string(bot_id)
                                            + ". Start parsing json object with zones info.");

            zones_info = controller->get_zones_info(task_id, tracker_id, source_id, period, api_key, tracker,
                                                    tracker_requests_url, tracker_api_key, ref(ignored_zones));

            spdlog::get("env_logger")->debug(sys_info + "Bot id: " + std::to_string(bot_id)
                                            + ". Json object successfullt parsed. Zones number: "
                                            + std::to_string(zones_info.size()));

            // satisfying of this condition means empty campaign statistics (no clicks)
            if (zones_info.size() == 1 && zones_info[0].first == NO_CLICKS)
            {
                continue;
            }

            if (zones_info.size() == 0)
            {
                spdlog::get("actions_logger")->error(
                    sys_info + "Can't get zones info for campaign "
                    + tracker_id + " | " + source_id + ". Skip.");
                continue;
            }
        }

        {  // checking condition for zones
            spdlog::get("env_logger")->info(sys_info + "Start zones checking.");

            zones_to_act_string = _check_condition_for_zones(zones_info, parsed_condition, zones_to_act);

            spdlog::get("env_logger")->info(sys_info + "Zones were successfully checked.");

            if (zones_to_act_string.size() == 0)
            {
                spdlog::get("actions_logger")->info(
                    sys_info + "Condition is true for 0 zones. Campaign: "
                    + tracker_id + " | " + source_id);
                continue;
            }
        }

        {  // building handling result in json format and put it to redis
            std::string handling_result = "{\"campaign_id\": " + source_id + ", \"action\": " + action + ", \"ts\": \""
                                        + ts_name + "\", \"zones\": " + zones_to_act_string + ", \"api_key\": \""
                                        + api_key + "\", \"list\": \"" + list_to_add + "\", \"client_id\": \""
                                        + client_id + "\"}";

            spdlog::get("actions_logger")->info(
                sys_info + "Condition is true for "+ std::to_string(zones_to_act.size())
                + " zones. Campaign: " + tracker_id + " | " + source_id);

            _put_action(handling_result, task_id);
        }
    }

    delete controller;
    delete parsed_condition;
}

// processing bot
void _process_task(const std::string bot_id_str, const std::string task_id)
{
    const std::size_t bot_id = (size_t)std::stoi(bot_id_str);
    auto bot_info = database::get_bot_info(bot_id);
    std::size_t action = (std::size_t)std::stoi(bot_info["action"]);

    switch (action)
    {
        case START_CAMPAIGN:
        case STOP_CAMPAIGN:
            _check_campaign(bot_id, std::ref(bot_info), task_id);
            break;
        case INCLUDE_ZONE:
        case EXCLUDE_ZONE:
            _check_zones(bot_id, std::ref(bot_info), task_id);
            break;
        default:
            std::string sys_info = format_log_message(bot_id_str, task_id);
            spdlog::get("actions_logger")->error(sys_info + "Unknown action: " + std::to_string(action));
            break;
    }
}

void _worker_main_function(std::vector<std::string> &storage)
{
    spdlog::get("env_logger")->info("Initialize worker.");

    while (true)
    {
        std::unique_lock<std::mutex> unique_storage_mutex(storage_mutex);
        cond_var.wait(unique_storage_mutex, [&storage] { return !storage.empty(); });

        auto bot_id = *storage.begin();
        storage.erase(storage.begin());
        unique_storage_mutex.unlock();

        std::string task_id = uuid::generate_uuid_v4();
        std::string sys_info = format_log_message(bot_id, task_id);

        spdlog::get("actions_logger")->info(sys_info + "Get new task.");
        _process_task(bot_id, task_id);
        spdlog::get("actions_logger")->info(sys_info + "Finish task.");
    }
}

void start(const std::size_t workers_num)
{
    std::vector<std::string> tasks;
    std::vector<std::thread> workers_pool;

    spdlog::get("env_logger")->info("Create resources (mutexes, containers). Start to initializing workers.");

    for (size_t _ = 0; _ < workers_num; _++)
    {
        workers_pool.emplace_back(_worker_main_function, ref(tasks));
        (*(workers_pool.end() - 1)).detach();
    }

    spdlog::get("env_logger")->info("Initialize and detach workers. Start storage updating...");

    queue_updating_process(ref(tasks));
}
