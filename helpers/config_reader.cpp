
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "config_reader.h"
using namespace std;

unordered_map<string, string> read_config(string filename)
{
    unordered_map<string, string> result;
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
            result.insert({key, value});
        }
    }

    return result;
}
