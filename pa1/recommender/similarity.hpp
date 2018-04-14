#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include <vector>
#include <math.h>
#include <unordered_map>
#include <numeric>

using namespace std;

template <typename T>
double pearson(const T& A, const T& B)
{
    int count{0};
    double rai{0};
    double rbi{0};
    double rabi{0};
    double mean_a{0};
    double mean_b{0};

    for(auto it=A.begin(); it!=A.end(); it++)
        mean_a += get<0>(A[it->first]);
    mean_a /= A.size();

    for(auto it=B.begin(); it!=B.end(); it++)
        mean_b += get<0>(A[it->first]);
    mean_b /= B.size();

    for(auto it=B.begin(); it!=B.end(); it++)
    {
        if (find(A[it->first]) != A.end())
        {
            rai  += A[it->first] - mean_a;
            rbi  += B[it->first] - mean_b;
            rabi += rai * rbi;
        }
    }

    return sqrt(rabi)/(sqrt(pow(rai, 2))*sqrt(pow(rai, 2)));
}

template <typename T>
double cosine_similarity(const T& A, const T& B)
{
    double rai{0};
    double rbi{0};
    double rabi{0};

    for(auto it=B.begin(); it!=B.end(); it++)
    {
        if (find(A[it->first]) != A.end())
        {
            rai  += A[it->first];
            rbi  += B[it->first];
            rabi += rai * rbi;
        }
    }

    return sqrt(rabi)/(sqrt(pow(rai, 2))*sqrt(pow(rai, 2)));
}

#endif // SIMILARITY_HPP
