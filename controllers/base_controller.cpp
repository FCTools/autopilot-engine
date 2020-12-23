#include <fstream>
#include <string>
#include <sstream>

#include "base_controller.h"

using namespace std;

BaseController::BaseController() 
{
    this->read_config("config.env");
}

void BaseController::read_config(string filename)
{
    fstream config(filename);

    string line;
    while(getline(config, line))
    {
        istringstream is_line(line);
        string key;

        if(getline(is_line, key, '='))
        {
            string value;
            if(getline(is_line, value)) 
            this->config.insert({key, value});
        }
    }
}
