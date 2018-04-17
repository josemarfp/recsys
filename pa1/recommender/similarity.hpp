#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include <vector>
#include <math.h>
#include <map>
#include <numeric>

using namespace std;

template <typename T>
double pearson(const T& A, const T& B)
{
    double rai{0};
    double rbi{0};
    double rabi{0};
    double mean_a{0};
    double mean_b{0};

    for(auto it=A.begin(); it!=A.end(); ++it)
        mean_a += it->first;
    mean_a /= A.size();

    for(auto it=B.begin(); it!=B.end(); ++it)
        mean_b += it->first;
    mean_b /= B.size();

    for(auto it=B.begin(); it!=B.end(); ++it)
    {
        if (A.find(it->first) != A.end())
        {
            double auxa;
            long auxatime;
            tie(auxa, auxatime) = A.at(it->first);
            auxa -= mean_a;

            double auxb;
            long auxbtime;
            tie(auxb, auxbtime) = B.at(it->first);
            auxb -= mean_b;

            rai  += auxa * auxa;
            rbi  += auxb * auxb;
            rabi += auxa * auxb;
        }
    }

    if (rabi != 0)
        return sqrt(rabi)/(sqrt(pow(rai, 2))*sqrt(pow(rbi, 2)));
    else
        return 0;
}

template <typename T>
double cosine_similarity(const T& A, const T& B)
{
    double rai{0};
    double rbi{0};
    double rabi{0};

    for(auto it=B.begin(); it!=B.end(); ++it)
    {
        if (A.find(it->first) != A.end())
        {
            double aux1;
            long aux1time;
            tie(aux1, aux1time) = A.at(it->first);

            double aux2;
            long aux2time;
            tie(aux2, aux2time) = B.at(it->first);

            rai  += aux1 * aux1;
            rbi  += aux2 * aux2;
            rabi += aux1 * aux2;
        }
    }

    if (rabi != 0)
        return sqrt(rabi)/(sqrt(pow(rai, 2))*sqrt(pow(rbi, 2)));
    else
        return 0;
}

template <typename T, typename U, typename V, typename W, typename Y>
double knn (T& user_item_ratings, T& item_user_ratings, U& k, V& result, Y& id, W& target, double (*f)(const unordered_map<int, tuple<double, long>>&, const unordered_map<int, tuple<double, long>>&))
{
    auto owned_items = user_item_ratings[id];
    //auto owned_users = item_user_ratings[target.first];
    double sim = 0;
    double num = 0;
    double dem = 0;

    for (auto owned_item : owned_items)
    {
        // Calculates similarity
        sim = f(item_user_ratings[target.first], item_user_ratings[owned_item.first]);

        if (sim > 0)
            result.insert(make_pair(sim, owned_item.first));

        if (result.size() > k)
        {
            auto it2 = result.begin();
            result.erase(result.begin(), ++it2);
        }

        num += sim * get<0>(owned_item.second);
        dem += sim;
    }

    if (dem != 0)
        return num/dem;
    else
        return 0;
}

#endif // SIMILARITY_HPP
