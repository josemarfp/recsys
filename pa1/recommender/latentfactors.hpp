#include <vector>
#include <math.h>
#include <unordered_map>
#include <numeric>

using namespace std;

template <typename T, typename U>
void plus(const T& A, const U& factor)
{
    T B;
    for(typename T::iterator it; it != A.end(); ++it)
        B[it->first] = A[it->first] * factor;
    return B;
}

template <typename T>
double dot(const T& A, const T& B)
{
    double result = 0;
    int k = B.size();

    for (int i = 0; i < k; i++)
        result += A.at(i) * B.at(i);

    return result;
}

/*!
 * Calulates prediction error
 */
template <typename T, typename U, typename V, typename X>
V error(const X& R, const T& P, const U& Q, const V& lambda)
{
    V erro = 0;

    for (auto rowit = R.begin(); rowit != R.end(); ++rowit)
    {
        V pu = dot(P.at(rowit->first), P.at(rowit->first));

        for (auto colit = rowit->second.begin(); colit != rowit->second.end(); ++colit)
        {
            V rui;
            long auxtime;
            tie(rui, auxtime) = R.at(rowit->first).at(colit->first);

            V qi = dot(Q.at(colit->first), Q.at(colit->first));

            V puqi = dot(P.at(rowit->first), Q.at(colit->first));

            erro += pow(rui - puqi, 2) + lambda * (pow(pu, 2) + pow(qi, 2));
        }
    }
    return erro;
}

/*!
 * Root Main Square Error
 */
template <typename T, typename U, typename V, typename X, typename V1>
V rmse(const X& R, const T& P, const U& Q, const V& lambda, const V1& lines, V (*f)(const X&, const T&, const U&, const V&))
{
    return sqrt(f(R, P, Q, lambda)/lines);
}

/*!
 * Breaks Ratings matrix in k latent factors
 */
template <typename T, typename U, typename V>
void initializeLatentFactors(T& A, U& lambda, V& k)
{
    for (auto row : A)
        for (unsigned int i=0; i < k; i++)
            lambda[row.first][i] = (double) (rand() % 100)/100;
}

/*!
 * Stocastic Gradient Descent
 */
template <typename T, typename U, typename V, typename X, typename V1>
void sgd(X& R, T& P, U& Q, const V& lambda, const V& gamma, const V1& lines, const V1& steps, V (*f)(const X&, const T&, const U&, const V&))
{
    double erro = rmse(R, P, Q, lambda, lines, f);

    for (unsigned int j = 1; j < steps; j++)
    {
        for (auto rowit = R.begin(); rowit != R.end(); ++rowit)
        {
            for (auto colit = rowit->second.begin(); colit != rowit->second.end(); ++colit)
            {
                V rui;
                long auxtime;
                tie(rui, auxtime) = R.at(rowit->first).at(colit->first);

                V puqi = dot(P.at(rowit->first), Q.at(colit->first));
                V eui = rui - puqi;

                for(unsigned int i = 0; i < P.at(rowit->first).size(); i++)
                {
                    V delta = 2*gamma*(eui*Q[colit->first][i] - lambda*P[rowit->first][i]);

                    P[rowit->first][i] = P[rowit->first][i] + delta;

                    delta = 2*gamma*(eui*Q[colit->first][i] - lambda*P[rowit->first][i]);

                    Q[colit->first][i] = Q[colit->first][i] + delta;

                    if (Q[colit->first][i] > 1000000)
                        cout << "Muito grande!";
                }
            }                
        }

        erro = rmse(R, P, Q, lambda, lines, f);
        //cout << erro << endl;
        if (erro < 0.5)
            break;
    }
}
