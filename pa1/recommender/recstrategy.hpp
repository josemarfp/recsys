#ifndef RECSTRATEGY_HPP
#define RECSTRATEGY_HPP

#include <unordered_map>
#include <map>
#include <tuple>
#include <iostream>
#include <string>

#include "latentfactors.hpp"
#include "similarity.hpp"

using namespace std;

/*!
 * \brief itemAvgRating. Left more than 15 minutes
 * \fn
 * \param users_items
 * \param key
 * \return
 *
 */
double globalAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items);

double itemAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items, int key);

double userAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users, int key);

void latent_factors
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& ratingsT,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
    unsigned int metric,
    unsigned int lines
);

void nearest_neighbors
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& user_item_ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& item_user_ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
    int metric
);

void randomize
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets
);

void runStrategy(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users,
                 unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
                 double (*f)(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users, int key));

void runStrategy(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items,
                 unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
                 double (*f)(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items));


#endif // RECSTRATEGY_HPP
