#pragma once

#include <cstdlib>
#include <string>
#include <pqxx/pqxx>

using namespace std;

class DatabaseClient
{
    string database_name;
    string database_host;
    string database_port;
    string database_user;
    string database_password;
    string connection_string;

    const string DEFAULT_PORT="5432";

public:

    DatabaseClient();

    string get_bot_condition(size_t bot_id);
};