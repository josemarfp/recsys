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
    V erro = 0;

    for (auto rowit = R.begin(); rowit != R.end(); ++rowit)
    {
        V pu{ 0 };
        dot(P[rowit->first], P[rowit->first], pu);

        for (auto colit = rowit->second.begin(); colit != rowit->second.end(); ++colit)
        {
            V rui = get<0>(R[rowit->first][colit->first]);

            V qi{ 0 };
            dot(Q[colit->first], Q[colit->first], qi);

            V puqi{ 0 };
            dot(P[rowit->first], Q[colit->first], puqi);

            if (puqi == NAN) {
                cout << "R[" << rowit->first << "][" << colit->first << "]  = " << get<0>(R[rowit->first][colit->first]) << endl;
                cout << "P[" << rowit->first << "][" << 0 << "]" << endl;
                cout << "Q[" << colit->first << "][" << 0 << "]" << endl;
            }

            erro += pow(rui - puqi, 2) + lambda * (pow(pu, 2) + pow(qi, 2));
        }
    }
    return erro;
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
void sgd(X& R, T& P, U& Q, V& lambda, V& gamma, V1& lines, V1& steps, V (*f)(X&, T&, U&, V&))
{
    double erro = rmse(R, P, Q, lambda, lines, f);
    for (unsigned int j = 1; j < steps; j++)
    {
        for (auto rowit = R.begin(); rowit != R.end(); ++rowit)
        {
            for (auto colit = rowit->second.begin(); colit != rowit->second.end(); ++colit)
            {
                V rui = get<0>(R[rowit->first][colit->first]);
                V puqi{0};
                dot(P[rowit->first], Q[colit->first], puqi);
                V eui = rui - puqi;

                if (eui != eui) {
                    cout << "R[" << rowit->first << "][" << colit->first << "]  = " << get<0>(R[rowit->first][colit->first]) << endl;
                    cout << "P[" << rowit->first << "][" << 0 << "]" << endl;
                    cout << "Q[" << colit->first << "][" << 0 << "]" << endl;
                }

                for(unsigned int i = 0; i < rowit->second.size(); i++)
                {
                    P[rowit->first][i] = P[rowit->first][i] + 2*gamma*(eui*Q[colit->first][i] - lambda*P[rowit->first][i]);
                    Q[colit->first][i] = Q[colit->first][i] + 2*gamma*(eui*P[rowit->first][i] - lambda*Q[colit->first][i]);
                }
            }                
        }

        erro = rmse(R, P, Q, lambda, lines, f);
        cout << erro << endl;
        if (erro < 0.5)
            break;
    }
}
