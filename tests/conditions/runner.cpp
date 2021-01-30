#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

#include "conditions.h"
#include "conditions_parser.h"


// statistics formulas
#define PROFIT(revenue, cost) (cost != 0) ? (revenue - cost) : 0.
#define ROI(revenue, cost) (cost != 0) ? ((revenue / cost - 1) * 100) : 0.
#define CR(leads, clicks) (clicks != 0) ? (100 * leads * 1.0 / clicks) : 0.
#define EPC(revenue, clicks) (clicks != 0) ? (revenue / clicks) : 0.
#define CPC(cost, clicks) (clicks != 0) ? (cost / clicks) : 0.
#define CPA(cost, leads) (leads != 0) ? (cost / leads) : 0.

using namespace std;

bool process_case(string filename)
{
    ifstream input(filename);
    string condition, tmp, answer;
    double cost, revenue;
    int leads, clicks;
    size_t pos;

    getline(input, condition);

    // read cost
    getline(input, tmp);
    pos = tmp.find("cost: ");
    cost = stod(tmp.substr(pos + strlen("cost: "), tmp.length() - pos));

    // read revenue
    getline(input, tmp);
    pos = tmp.find("revenue: ");
    revenue = stod(tmp.substr(pos + strlen("revenue: "), tmp.length() - pos));

    // read clicks
    getline(input, tmp);
    pos = tmp.find("clicks: ");
    clicks = stoi(tmp.substr(pos + strlen("clicks: "), tmp.length() - pos));

    // read leads
    getline(input, tmp);
    pos = tmp.find("leads: ");
    leads = stoi(tmp.substr(pos + strlen("leads: "), tmp.length() - pos));

    // read answer
    getline(input, tmp);
    pos = tmp.find("answer: ");
    answer = tmp.substr(pos + strlen("answer: "), tmp.length() - pos);

    input.close();

    ConditionsParser parser;
    auto parsed_condition = parser.parse_condition(condition);

    unordered_map<string, double> stat = {{"cost", cost},
                                          {"revenue", revenue},
                                          {"clicks", clicks},
                                          {"leads", leads},
                                          {"profit", PROFIT(revenue, cost)},
                                          {"ROI", ROI(revenue, cost)},
                                          {"CR", CR(leads, clicks)},
                                          {"EPC", EPC(revenue, clicks)},
                                          {"CPC", CPC(cost, clicks)},
                                          {"CPA", CPA(cost, leads)}};

    string answ = "false";

    if (parsed_condition->is_true(stat))
    {
        answ = "true";
    }

    return answ == answer;
}

int main()
{
    const size_t cases_number = 36;

    for (size_t i = 0; i < cases_number; i++)
    {
        bool passed = process_case("../cases/" + to_string(i) + ".txt");
        if (passed)
        {
            cout << "Test " + to_string(i) + ":"
                 << " passed" << endl;
        }
        else
        {
            cout << "Test " + to_string(i) + ":"
                 << " failed" << endl;
        }
        
    }

    return EXIT_SUCCESS;
}
