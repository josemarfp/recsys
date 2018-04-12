#ifndef RECSTRATEGY_HPP
#define RECSTRATEGY_HPP

#include <unordered_map>
#include <tuple>

using namespace std;
/*!
 * \brief itemAvgRating. Left more than 15 minutes
 * \fn
 * \param users_items
 * \param key
 * \return
 *
 */
double itemAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items, int key);
double userAvgRating(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users, int key);
void runStrategy(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users,
                 unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
                 double (*f)(unordered_map<int, unordered_map<int, tuple<double, long>>>& items_users, int key));

#endif // RECSTRATEGY_HPP
