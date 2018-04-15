#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include <ctime>

#include "filemanip.hpp"
#include "recstrategy.hpp"
#include "latentfactors.hpp"
#include "similarity.hpp"

using namespace std;

void nearest_neighbors
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& user_item_ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& item_user_ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
    int metric
)
{
    unsigned int neighbors = 30;

    for (auto target : targets)
    {
        for (auto item_i : target.second)
        {
            multimap<double, int> nearest;
            switch (metric) {
                case 1:
                {
                    targets[target.first][item_i.first] = make_pair(knn(user_item_ratings, item_user_ratings, neighbors, nearest, target.first, item_i, pearson), clock());
                    break;
                }
                case 2:
                {
                    targets[target.first][item_i.first] = make_pair(knn(user_item_ratings, item_user_ratings, neighbors, nearest, target.first, item_i, cosine_similarity), clock());
                    break;
                }
            }
        }

        //for (auto it=nearest.rbegin()++; it != nearest.rend(); ++it)
        //    cout << it->second << ":" << it->first << endl;
    }

}

void latent_factors
(
    unordered_map<int, unordered_map<int, tuple<double, long>>>& ratings,
    unordered_map<int, unordered_map<int, tuple<double, long>>>& targets,
    int metric,
    int lines
)
{
    // Latent Factors Model
    unordered_map<int, unordered_map<int, double>> P;
    unordered_map<int, unordered_map<int, double>> Q;
    int k{4}; // number of latent factors
    double lambda{0.003};
    double gamma{0.02};
    int steps = 10;

    initializeLatentFactors(ratings, P, Q, k);

    switch (metric) {
        case 1:
        {
            sgd(ratings, P, Q, lambda, gamma, lines, steps, error);
            break;
        }
    }

    //cout << error(ratings, P, Q, lambda) << endl;
    //cout << rmse(ratings, P, Q, lambda, lines, error) << endl;
}

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
    int method = argc >= 3 ? std::stoi(argv[3]) : 1;
    int metric = argc >= 3 ? std::stoi(argv[4]) : 2;
    bool itemBased = argc >= 3 ? std::stoi(argv[5]) == 1 : false;

    // Load data
    string submission_fileName{"./../dataset/submission-rand.csv"};

    unordered_map<int, unordered_map<int, tuple<double, long>>> user_item_ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> item_user_ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> targets{};

    int users = file2Container(user_item_ratings, ratings_fileName, parseRatings, false);
    int items = file2Container(item_user_ratings, ratings_fileName, parseRatings, true);
    file2Container(targets, targets_fileName, parseTargets, itemBased);

    // Run desired method
    switch (method)
    {
        case 1:
        {
            nearest_neighbors(user_item_ratings, item_user_ratings, targets, metric);
            break;
        }
        case 2:
        {
            latent_factors(user_item_ratings, targets, metric, users);
            break;
        }
        default:
        {
            break;
        }
    }

    coutRecommendation(targets);

    // finish time
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Code took " << elapsed_secs << " seconds to execute\n";

    return 0;
}

