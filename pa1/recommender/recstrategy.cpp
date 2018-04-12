#include "recstrategy.hpp"

using namespace std;

double itemAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items, int key)
{
    int count{0};
    int accum{0};
    for (auto it_user = users_items.begin(); it_user != users_items.end(); ++it_user)
    {
        auto it_item = it_user->second.find(key);
        if (it_item != it_user->second.end())
        {
            count++;
            accum += std::get<0>(it_item->second);
        }
    }
    return count > 0 ? accum / count : 0;
}

double userAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users, int key)
{
    int count{0};
    int accum{0};
    for (auto it_item = items_users[key].begin(); it_item != items_users[key].end(); ++it_item)
    {
        count++;
        accum += std::get<0>(it_item->second);
    }
    return count > 0 ? accum / count : 0;
}

void runStrategy(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items,
                 unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
                 double (*f)(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items, int key))
{
    for (auto it = targets.begin(); it != targets.end(); ++it)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            std::get<0>(targets[it->first][it2->first]) = f(users_items, it2->first);
}
