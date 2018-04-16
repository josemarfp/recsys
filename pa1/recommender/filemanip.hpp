#ifndef FILNEMANIP_HPP
#define FILNEMANIP_HPP

#include <unordered_map>
#include <list>
#include <string>

using namespace std;

/*!
 * \brief Iterate a text file and put its content in a container
 * \param container
 * \param fileName
 * \return
 */
int file2Container(unordered_map<int, unordered_map<int, tuple<double, long>>> & container,
                   string &fileName,
                   tuple<int, int, tuple<double, long>> (*f)(string &line),
                   bool items_users);

/*!
 * \brief Parses a line in ratings.csv file.
 * \param users
 * \param line
 * \return
 */
tuple<int, int, tuple<double, long>> parseTargets(string &line);

/*!
 * \brief Parses a line in ratings.csv file.
 * \param users
 * \param line
 * \return
 */
tuple<int, int, tuple<double, long>> parseRatings(string &line);

/*!
 * \brief coutRecommendation
 * \param users_items
 */
void coutRecommendation(unordered_map<int, unordered_map<int, tuple<double, long>>>& users_items);

/*!
 *
 */
template <typename T>
T transpose(T& A)
{
    T AT;
    for (auto it = A.begin(); it != A.end(); it++)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            AT[it2->first][it->first] = A[it->first][it2->first];
    return AT;
}

#endif // FILNEMANIP_HPP
