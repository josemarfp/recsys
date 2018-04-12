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

template <typename T, typename U>
void dot(T& A, T& B, U& result)
{
    result = 0;
    int k = B.size();
    for (int i = 0; i < k; i++)
        result += A[i] * B[i];
        //result += A[i] != A[i] ? 1 : A[i] * B[i] != B[i] ? 1 : B[i];
        // The ternary is checking if is NaN
}

/*!
 * Calulates prediction error
 */
template <typename T, typename U, typename V, typename X>
V error(X& R, T& P, U& Q, V& lambda)
{
    V error = 0;

    for (auto rowit=R.begin(); rowit!=R.end(); rowit++)
        for (auto colit=rowit->second.begin(); colit!=rowit->second.end(); colit++)
        {
            V rui = get<0>(R[rowit->first][colit->first]);

            V pu{ 0 };
            dot(P[rowit->first], P[rowit->first], pu);

            V qi{ 0 };
            if (Q[colit->first][0] != Q[colit->first][0]) {
                cout << rowit->first << ";" << colit->first << ":" << Q[colit->first][0] << endl;

            }

            dot(Q[colit->first], Q[colit->first], qi);

            V puqi{ 0 };
            dot(P[rowit->first], Q[colit->first], puqi);

            error += pow(rui - puqi, 2) + lambda * (pow(pu, 2) + pow(qi, 2));
        }
    return error;
}

/*!
 * Root Main Square Error
 */
template <typename T, typename U, typename V, typename X, typename V1>
V rmse(X& R, T& P, U& Q, V& lambda, V1& lines, V (*f)(X&, T&, U&, V&))
{
    return sqrt(f(R, P, Q, lambda)/lines);
}

/*!
 * Breaks Ratings matrix in k latent factors
 */
template <typename T, typename U, typename V, typename X>
void initializeLatentFactors(X& R, T& P, U& Q, V& k)
{
    for (auto rowit=R.begin(); rowit!=R.end(); rowit++)
    {
        for (int i=0; i < k; i++)
            P[rowit->first][i] = (double) (rand() % 100)/100;

        for (auto colit=rowit->second.begin(); colit!=rowit->second.end(); colit++)
            for (int i=0; i < k; i++) {
                Q[colit->first][i] = (double) (rand() % 100)/100;
                if (colit->first == 2562232 and rowit->first == 833)
                    cout << i << ": " << Q[colit->first][i] << endl;

            }
    }
}

/*!
 * Stocastic Gradient Descent
 */
template <typename T, typename U, typename V, typename X, typename V1>
void sgd(X& R, T& P, U& Q, V& lambda, V& gamma, V1& lines, V1& steps, V (*f)(X&, T&, U&, V&))
{
    double erro = rmse(R, P, Q, lambda, lines, f);
    for (int i = 1; i < steps; i++)
    {
        for (auto rowit=R.begin(); rowit!=R.end(); rowit++)
            for (auto colit=rowit->second.begin(); colit!=rowit->second.end(); colit++)
            {
                V rui = get<0>(R[rowit->first][colit->first]);

                if (rui > 0)
                {
                    V puqi{ 0 };
                    dot(P[rowit->first], Q[colit->first], puqi);

                    V eui = rui - puqi;

                    for(unsigned int i = 0; i < P[rowit->first].size(); i++)
                    {
                        double p = P[rowit->first][i]+2*gamma*(eui*Q[colit->first][i] - lambda*P[rowit->first][i]);
                        P[rowit->first][i] = p;
                        double q = Q[colit->first][i]+2*gamma*(eui*P[rowit->first][i] - lambda*Q[colit->first][i]);
                        Q[colit->first][i] = q;
                        //debug: cout << "P[" << rowit->first << "," << i << "]=" << P[rowit->first][i] << endl;
                        //debug: cout << "Q[" << colit->first << "," << i << "]=" << Q[colit->first][i] << endl;
                    }
                }

            }
        erro = rmse(R, P, Q, lambda, lines, f);
        cout << erro << endl;
        if (erro < 0.5)
            break;
    }
}
