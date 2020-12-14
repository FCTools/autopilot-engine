#include <cstdlib>
#include <string>
#include <pqxx/pqxx>

#include "database_client.h"

using namespace std;

DatabaseClient::DatabaseClient() 
{
    this->database_name = string(getenv("DATABASE_NAME"));
    this->database_host = string(getenv("DATABASE_HOST"));
    this->database_port = string(getenv("DATABASE_PORT"));
    this->database_user = string(getenv("DATABASE_USER"));
    this->database_password = string(getenv("DATABASE_PASSWORD"));

    this->connection_string = "dbname=" + database_name + " host=" + database_host + " port=" + database_port + " user=" + database_user + " password=" + database_password;
}

string DatabaseClient::get_bot_condition(size_t bot_id)
{
    // pqxx::connection connection(this->connection_string);
    return string();        
}

