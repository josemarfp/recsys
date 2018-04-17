#include "recstrategy.hpp"

using namespace std;

double globalAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items)
{
    int count{0};
    double accum{0};
    for (auto rowit = users_items.begin(); rowit != users_items.end(); ++rowit)
    {
        for (auto colit = rowit->second.begin(); colit != rowit->second.end(); ++colit)
        {
            count++;
            accum += std::get<0>(colit->second);
        }
    }
    return count > 0 ? accum / count : 0;
}

double itemAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items, int key)
{
    int count{0};
    double accum{0};
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
    double accum{0};
    for (auto it_item = items_users[key].begin(); it_item != items_users[key].end(); ++it_item)
    {
        count++;
        accum += std::get<0>(it_item->second);
    }
    return count > 0 ? accum / count : 0;
}

void randomize
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets
)
{
    for (auto target : targets)
    {
        for (auto item_i : target.second)
        {
            /* initialize random seed: */
            if (get<0>(targets[target.first][item_i.first]) == 0)
            {
                srand(time(nullptr));
                targets[target.first][item_i.first] = make_pair((double) (rand() % (100+1))/10, clock());
            }
        }
    }

}

void nearest_neighbors
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& user_item_ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& item_user_ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
    int metric
)
{
    unsigned int neighbors = 20;

    for (auto target : targets)
    {
        for (auto item_i : target.second)
        {
            multimap<double, int> nearest;
            switch (metric) {
                case 1:
                {
                    double rui = knn(user_item_ratings, item_user_ratings, neighbors, nearest, target.first, item_i, pearson);
                    if ((rui != 0) && (get<0>(targets[target.first][item_i.first]) == 0))
                        targets[target.first][item_i.first] = make_pair(rui, clock());
                    break;
                }
                case 2:
                {
                    double rui = knn(user_item_ratings, item_user_ratings, neighbors, nearest, target.first, item_i, cosine_similarity);
                    if ((rui != 0) && (get<0>(targets[target.first][item_i.first]) == 0))
                        targets[target.first][item_i.first] = make_pair(rui, clock());
                    break;
                }
            }
        }
    }

}

void latent_factors
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& ratingsT,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
    unsigned int metric,
    unsigned int lines
)
{
    // Latent Factors Model
    unordered_map<int, unordered_map<int, double>> P;
    unordered_map<int, unordered_map<int, double>> Q;
    unsigned int k{30}; // number of latent factors
    double lambda{0.001};
    double gamma{0.001};
    unsigned int steps = 25;

    initializeLatentFactors(ratings,  P, k);
    initializeLatentFactors(ratingsT, Q, k);

    switch (metric) {
        case 1:
        {
            sgd(ratings, P, Q, lambda, gamma, lines, steps, error);
            break;
        }
    }

    for (auto target : targets)
        for (auto item_i : target.second)
        {
            if (P.find(target.first) != P.end() && Q.find(item_i.first) != Q.end())
            {
                double rui = dot(P[target.first], Q[item_i.first]);
                targets[target.first][item_i.first] = make_pair(rui > 10 ? 10 : rui < 0 ? 0 : rui, clock());
            }
        }
}

void runStrategy(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items,
                 unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
                 double (*f)(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items, int key))
{
    for (auto it = targets.begin(); it != targets.end(); ++it)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            if (std::get<0>(targets[it->first][it2->first]) == 0)
                std::get<0>(targets[it->first][it2->first]) = f(users_items, it2->first);
}

void runStrategy(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items,
                 unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
                 double (*f)(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items))
{
    double rui = f(users_items);
    for (auto it = targets.begin(); it != targets.end(); ++it)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            if (std::get<0>(targets[it->first][it2->first]) == 0)
                std::get<0>(targets[it->first][it2->first]) = rui;
}
