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
#include <set>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "redis_client.h"
#include "database_client.h"
#include "conditions.h"
#include "conditions_parser.h"
#include "binom_client.h"
#include "base_controller.h"
#include "propeller_controller.h"
#include "evadav_controller.h"
#include "mgid_controller.h"
#include "http.h"
#include "main_loop.h"

const size_t checking_timeout = (size_t)stoi(std::string(getenv(
                                                        "CHECKING_TIMEOUT")));
mutex storage_mutex, actions_mutex;
condition_variable cond_var;

// uuid generating logic
namespace uuid {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 15);
    static uniform_int_distribution<> dis2(8, 11);

    string generate_uuid_v4() {
        stringstream ss;
        ss << hex;

        for (size_t _ = 0; _ < 8; _++) {
            ss << dis(gen);
        }
        ss << "-";

        for (size_t _ = 0; _ < 4; _++) {
            ss << dis(gen);
        }
        ss << "-4";

        for (size_t _ = 0; _ < 3; _++) {
            ss << dis(gen);
        }
        ss << "-" << dis2(gen);

        for (size_t _ = 0; _ < 3; _++) {
            ss << dis(gen);
        }
        ss << "-";

        for (size_t _ = 0; _ < 12; _++) {
            ss << dis(gen);
        }

        return ss.str();
    }
}  // namespace uuid

std::set<std::string> _split(std::string source, char delimiter) {
    if (source.find(delimiter) == std::string::npos) {
	    return { source };
	}

	size_t current = 0, next;
	std::set<std::string> tokens;

	while (source.find(delimiter, current) != std::string::npos) {
		next = source.find(delimiter, current);
		tokens.insert(source.substr(current, next - current));
		current = next + 1;
	}
	tokens.insert(source.substr(current, source.length() - current));

	return tokens;
}

void _extend_storage(std::vector<std::string>& storage,
                     std::vector<std::string>& new_tasks) {
    lock_guard<mutex> lock_(storage_mutex);
    storage.reserve(storage.size()
                    + distance(new_tasks.begin(), new_tasks.end()));
    storage.insert(storage.end(), new_tasks.begin(), new_tasks.end());
}

// check for new tasks in storage redis
void _start_queue_updating_process(std::vector<std::string>& tasks) {
    RedisClient redis;

    while (true) {
        auto updates = redis.get_updates();

        if (!updates.empty()) {
            _extend_storage(tasks, updates);
            cond_var.notify_all();
        }

        this_thread::sleep_for(chrono::seconds(checking_timeout));
    }
}

void _put_action(RedisClient& redis, const std::string data) {
    auto task_id = uuid::generate_uuid_v4();

    lock_guard<mutex> lock_(actions_mutex);
    redis.put_action(task_id, data);
}

BaseController* _get_controller(std::string ts) {
    if (ts == "Propeller Ads") {
        return new PropellerController();
    } else if (ts == "Evadav") {
        return new EvadavController();
    } else if (ts == "MGID") {
        return new MgidController();
    }
    else
    {
        spdlog::get("env_logger")->error("Can't choose controller for"
                                         " traffic source " + ts);
        return nullptr;
    }
}

void _check_campaign(const size_t bot_id,
                     std::unordered_map<std::string, std::string>& bot_info) {
    // TODO: put this value to env variables
    const size_t default_tries = 5;
    size_t current_tries;

    RedisClient redis;

    auto condition = bot_info["condition"];
    size_t period = (size_t)stoi(bot_info["period"]);
    auto api_key = bot_info["api_key"];
    auto ts = bot_info["ts"];
    auto action = bot_info["action"];

    // get bot campaigns from database
    auto campaigns_ids = database::get_bot_campaigns(bot_id);

    ConditionsParser parser;

    spdlog::get("env_logger")->info("Start condition parsing: " + condition);

    BaseCondition* parsed_condition = parser.parse_condition(condition);
    spdlog::get("env_logger")->info("Condition " + condition
                                    + " was successfully parsed");

    auto ts_name = database::get_ts_name((size_t)stoi(ts));
    BaseController* controller = _get_controller(ts_name);
    if (!controller) {
        return;
    }
    spdlog::get("env_logger")->info("Select controller for " + ts_name);

    // check bot campaigns
    for (size_t campaign_id : campaigns_ids) {
        auto ids = database::get_campaign_ids(campaign_id);
        size_t tracker_id = ids.first;
        auto source_id = ids.second;

        std::unordered_map<std::string, double> campaign_info;
        current_tries = default_tries;

        while (current_tries) {
            campaign_info = controller->get_campaign_info(tracker_id,
                                                          source_id,
                                                          period, api_key);

            if (campaign_info.size() != 0) {
                break;
            }
            current_tries--;
            this_thread::sleep_for(chrono::seconds(5));
        }

        if (campaign_info.size() == 0) {
            spdlog::get("actions_logger")->error("Bot id: " + to_string(bot_id)
                                                 + ". Can't get campaign info."
                                                 " Skip campaign: "
                                                 + to_string(campaign_id));
            continue;
        }

        if (parsed_condition->is_true(campaign_info)) {
            auto data = "{\"campaign_id\": " + source_id
                        + ", \"action\": " + action + ", \"ts\": \""
                        + ts_name + "\", \"api_key\": \""
                        + api_key + "\"}";

            spdlog::get("actions_logger")->info(
                "Bot id: " + to_string(bot_id) + ". Condition is true for "
                "campaign " + to_string(tracker_id) + " | " + source_id);
            _put_action(redis, data);
        }
    }

    delete controller;
}

void _check_zones(const size_t bot_id,
                  std::unordered_map<std::string, std::string>& bot_info) {
    // TODO: put this value to env variables
    const size_t default_tries = 5;
    size_t current_tries;

    RedisClient redis;

    auto condition = bot_info["condition"];
    size_t period = (size_t)stoi(bot_info["period"]);
    auto api_key = bot_info["api_key"];
    auto ts = bot_info["ts"];
    auto action = bot_info["action"];
    auto list_to_add = bot_info["list_to_add"];

    // get bot campaigns from database
    auto campaigns_ids = database::get_bot_campaigns(bot_id);
    auto ignored_zones = _split(bot_info["ignored_zones"], '\n');

    ConditionsParser parser;

    spdlog::get("env_logger")->info("Start condition parsing: " + condition);
    BaseCondition* parsed_condition = parser.parse_condition(condition);
    spdlog::get("env_logger")->info("Condition " + condition
                                    + " was successfully parsed");

    auto ts_name = database::get_ts_name((size_t)stoi(ts));
    BaseController* controller = _get_controller(ts_name);
    if (!controller) {
        return;
    }
    spdlog::get("env_logger")->info("Select controller for " + ts_name);

    std::vector<std::string> zones_to_act;

    for (size_t campaign_id : campaigns_ids) {
        auto ids = database::get_campaign_ids(campaign_id);
        size_t tracker_id = ids.first;
        auto source_id = ids.second;
        zones_data zones_info;

        current_tries = default_tries;

        while (current_tries) {
            spdlog::get("env_logger")->info(
                "Bot id: " + std::to_string(bot_id)
                + ". Start parsing json object with zones info.");

            zones_info = controller->get_zones_info(tracker_id, source_id,
                                                    period, api_key,
                                                    ref(ignored_zones));

            spdlog::get("env_logger")->info(
                "Bot id: " + std::to_string(bot_id)
                + ". Json object successfullt parsed. Zones number "
                + std::to_string(zones_info.size()));

            if (zones_info.size() != 0) {
                break;
            }
            current_tries--;
            this_thread::sleep_for(chrono::seconds(5));
        }

        if (zones_info.size() == 1 && zones_info[0].first == NO_CLICKS) {
            continue;
        }

        if (zones_info.size() == 0) {
            spdlog::get("actions_logger")->error(
                "Bot id: " + to_string(bot_id) + ". Can't get zones info for"
                " campaign " + to_string(tracker_id) + " | "
                + source_id + ". Skip.");
            continue;
        }

        spdlog::get("env_logger")->info(
                "Bot id: " + std::to_string(bot_id)
                + ". Start zones checking.");

        for (auto& zone : zones_info) {
            if (parsed_condition->is_true(zone.second)) {
                zones_to_act.push_back(zone.first);
            }
        }

        spdlog::get("env_logger")->info(
                "Bot id: " + std::to_string(bot_id)
                + ". Zones were successfully checked.");

        std::string zones_to_act_string = "[";
        for (auto zone : zones_to_act) {
            zones_to_act_string += "\"" + zone + "\",";
        }

        if (zones_to_act_string != "[") {
            zones_to_act_string = zones_to_act_string.substr(0,
                                            zones_to_act_string.length() - 1);
        } else {
            spdlog::get("actions_logger")->info(
                "Bot id: " + to_string(bot_id) + ". Condition is true for 0 z"
                "ones. Campaign: " + to_string(tracker_id) + " | " + source_id);
            continue;
        }
        zones_to_act_string += "]";

        std::string data = "{\"campaign_id\": " + source_id + ", \"action\": "
                           + action + ", \"ts\": \""
                           + ts_name + "\", \"zones\": " +
                           zones_to_act_string +", \"api_key\": \"" + api_key +
                           "\", \"list\": \"" + list_to_add + "\"}";

        spdlog::get("actions_logger")->info(
            "Bot id: " + to_string(bot_id) + ". Condition is true for "
            + to_string(zones_to_act.size()) + " zones. Campaign: "
            + to_string(tracker_id) + " | " + source_id);
        _put_action(redis, data);
    }

    delete controller;
}

// processing bot
void _process_task(const std::string bot_id_str) {
    const size_t bot_id = (size_t)stoi(bot_id_str);
    auto bot_info = database::get_bot_info(bot_id);
    size_t action = (size_t)stoi(bot_info["action"]);

    switch (action) {
        case START_CAMPAIGN:
        case STOP_CAMPAIGN:
            _check_campaign(bot_id, ref(bot_info));
            break;
        case INCLUDE_ZONE:
        case EXCLUDE_ZONE:
            _check_zones(bot_id, ref(bot_info));
            break;
        default:
            spdlog::get("actions_logger")->error("Unknown action: "
                                               + std::to_string(action));
            break;
    }
}

void _worker_main_function(std::vector<std::string>& storage) {
    spdlog::get("env_logger")->info("Initialize worker.");

    while (true) {
        unique_lock<mutex> unique_storage_mutex(storage_mutex);
		cond_var.wait(unique_storage_mutex,
                      [&storage]{return !storage.empty(); });

        auto bot_id = *storage.begin();
        storage.erase(storage.begin());
        unique_storage_mutex.unlock();

        spdlog::get("actions_logger")->info("Get new task. Bot id: " + bot_id);
        _process_task(bot_id);
        spdlog::get("actions_logger")->info("Finish task. Bot id: " + bot_id);
    }
}

void start(const size_t workers_num) {
    std::vector<std::string> tasks;
    std::vector<thread> workers_pool;

    spdlog::get("env_logger")->info("Create resources (mutexes, containers)."
                                    "Start to initializing workers.");

    for (size_t _ = 0; _ < workers_num; _++) {
        workers_pool.emplace_back(_worker_main_function, ref(tasks));
        (*(workers_pool.end() - 1)).detach();
    }

    spdlog::get("env_logger")->info("Initialize and detach workers."
                                    " Start storage updating...");

    _start_queue_updating_process(ref(tasks));
}
