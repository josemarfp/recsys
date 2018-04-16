#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <tuple>
#include "filemanip.hpp"


int file2Container(
        unordered_map<int, unordered_map<int, tuple<double, long>>>& container,
        string &fileName,
        tuple<int, int, tuple<double, long>> (*f)(string &line),
        bool items_users = false
        )
{
    ifstream refFile(fileName);
    string currentLine = "";
    int lines = 0;
    //container.reserve(10000000);              //!! stage 3

    while(getline(refFile, currentLine)) {
        if (lines > 0) // considera que a linha zero é o cabeçalho.
        {
            auto parsedLine = f(currentLine);     //!! stage 2
            if (items_users)
                container[std::get<1>(parsedLine)][std::get<0>(parsedLine)] = std::get<2>(parsedLine);
            else
                container[std::get<0>(parsedLine)][std::get<1>(parsedLine)] = std::get<2>(parsedLine);
        }
        lines++;
    }

    refFile.close();
        
    return lines;
}

void coutRecommendation(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items)
{
    cout << "UserId:ItemId,Prediction" << endl;
    for (auto it = users_items.begin(); it != users_items.end(); it++)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            cout << "u" << setfill('0') << setw(7) << it->first
                 << ":i" << setfill('0') << setw(7) << it2->first
                 << "," << std::get<0>(it2->second) << endl;
}


tuple<int, int, tuple<double, long>> parseTargets(string &line)
{
    // 1-7: userid
    // 10-16: itemid
    int user = std::stoi(line.substr(1, 7));  // Novo jeito de fazer o cast para inteiro
    int item = std::stoi(line.substr(10, 7)); // no cpp moderno
    double prediction = 0;
    long timestamp = 0;

    return std::make_tuple(user, item, std::make_tuple(prediction, timestamp));
}


tuple<int, int, tuple<double, long>> parseRatings(string &line)
{
    // 1-7: userid
    // 10-16: itemid
    // 18 or 18-19: prediction (from 1 to 10)
    // 20-30 or 19-29: timestamp
    int user = std::stoi(line.substr(1, 7));  // Novo jeito de fazer o cast para inteiro
    int item = std::stoi(line.substr(10, 7)); // no cpp moderno
    double prediction = line.substr(19, 1) == ";" ? std::stoi(line.substr(18, 1)) : std::stoi(line.substr(18, 2));
    long timestamp = line.substr(19, 1) == ";" ? std::stod(line.substr(20, 10)) : std::stod(line.substr(21, 10));

    return std::make_tuple(user, item, std::make_tuple(prediction, timestamp));
}




