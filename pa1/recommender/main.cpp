#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include <time.h>
#include <numeric>

#include "filemanip.hpp"
#include "recstrategy.hpp"


using namespace std;

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    if (argc >= 3)
    {
        cout << "argumentos: " << argc << endl;
        cout << "recommender " << argv[1] << " " << argv[2] << endl;
    }

    // Start time
    clock_t begin = clock();

    // Default parameter
    string ratings_fileName{argc >= 3 ? argv[1] : "./../dataset/ratings.csv"};
    string targets_fileName{argc >= 3 ? argv[2] : "./../dataset/targets.csv"};
    vector<int> methods = {2, 5, 4, 6}; // {1, 0, 3};
    vector<int> metrics = {1, 0, 0, 0}; // {2, 2, 0};
    long min_date = 1382915585;
    bool itemBased = false;

    // Load data
    string submission_fileName{"./../dataset/submission-rand.csv"};

    unordered_map<int, unordered_map<int, tuple<double, long>>> user_item_ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> item_user_ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> targets{};

    int users = file2Container(user_item_ratings, ratings_fileName, parseRatings, itemBased, min_date);
    item_user_ratings = transpose(user_item_ratings);
    file2Container(targets, targets_fileName, parseTargets, itemBased, 0);

    for (unsigned int i = 0; i < methods.size(); i++)
    {
        // Run desired method
        switch (methods[i])
        {
            case 0:
            {
                targets = transpose(targets);
                nearest_neighbors(item_user_ratings, user_item_ratings, targets, metrics[i]);
                targets = transpose(targets);
                break;
            }
            case 1:
            {
                nearest_neighbors(user_item_ratings, item_user_ratings, targets, metrics[i]);
                break;
            }
            case 2:
            {
                latent_factors(user_item_ratings, item_user_ratings, targets, metrics[i], users);
                break;
            }
            case 3:
            {
                randomize(targets);
                break;
            }
            case 4:
            {
                runStrategy(item_user_ratings, targets, userAvgRating);
                break;
            }
            case 5:
            {
                targets = transpose(targets);
                runStrategy(user_item_ratings, targets, itemAvgRating);
                targets = transpose(targets);
                break;
            }
            case 6:
            {
                runStrategy(user_item_ratings, targets, globalAvgRating);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    coutRecommendation(targets);

    // finish time
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Code took " << elapsed_secs << " seconds to execute\n";

    return 0;
}

