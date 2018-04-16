#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include <time.h>
#include <numeric>

#include "filemanip.hpp"
#include "recstrategy.hpp"
#include "latentfactors.hpp"
#include "similarity.hpp"

using namespace std;

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
    unsigned int neighbors = 10;

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
    unsigned int k{7}; // number of latent factors
    double lambda{0.003};
    double gamma{0.02};
    unsigned int steps = 10;

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
            double rui{0};
            dot(P[target.first], Q[item_i.first], rui);
            targets[target.first][item_i.first] = make_pair(rui, clock());
        }
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
    vector<int> methods = {2}; // {1, 0, 3};
    vector<int> metrics = {1}; // {2, 2, 0};
    bool itemBased = false;

    // Load data
    string submission_fileName{"./../dataset/submission-rand.csv"};

    unordered_map<int, unordered_map<int, tuple<double, long>>> user_item_ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> item_user_ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> targets{};

    int users = file2Container(user_item_ratings, ratings_fileName, parseRatings, itemBased);
    item_user_ratings = transpose(user_item_ratings);
    file2Container(targets, targets_fileName, parseTargets, itemBased);

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

