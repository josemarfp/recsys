#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <chrono>

#include "filemanip.hpp"
#include "recstrategy.hpp"
#include "latentfactors.hpp"

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
    std::chrono::high_resolution_clock::time_point start, end;

    string ratings_fileName{argc >= 3 ? argv[1] : "./../dataset/ratings.csv"};
    string targets_fileName{argc >= 3 ? argv[2] : "./../dataset/targets.csv"};

    string submission_fileName{"./../dataset/submission-rand.csv"};

    unordered_map<int, unordered_map<int, tuple<double, long>>> ratings{};
    unordered_map<int, unordered_map<int, tuple<double, long>>> targets{};

    int lines = file2Container(ratings, ratings_fileName, parseRatings, false);
    file2Container(targets, targets_fileName, parseTargets, false);

    // Latent Factors Model
    unordered_map<int, unordered_map<int, double>> P;
    unordered_map<int, unordered_map<int, double>> Q;
    int k{4}; // number of latent factors
    double lambda{0.003};
    double gamma{0.02};
    int steps = 10;

    initializeLatentFactors(ratings, P, Q, k);
    sgd(ratings, P, Q, lambda, gamma, lines, steps, error);
    //cout << error(ratings, P, Q, lambda) << endl;
    //cout << rmse(ratings, P, Q, lambda, lines, error) << endl;

    //runStrategy(ratings, targets, itemAvgRating);
    //runStrategy(users_items, targets, userAvgRating);

    //coutRecommendation(targets);

    // finish time
    end = std::chrono::high_resolution_clock::now();

    double time_elapsed = std::chrono::duration_cast<std::chrono::seconds>( end - start ).count();
    std::cout << "Code took " << time_elapsed << " seconds to execute\n";

    return 0;
}



unsigned hamdist(unsigned x, unsigned y)
{
  unsigned dist = 0, val = x ^ y;

  // Count the number of set bits
  while(val)
  {
    ++dist;
    val &= val - 1;
  }

  return dist;
}
