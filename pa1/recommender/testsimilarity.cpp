#include <QtTest/QtTest>
#include <unordered_map>
#include <iostream>
#include "similarity.hpp"

using namespace std;

class TestSimilarity : public QObject
{
    Q_OBJECT
    private slots:
    void testpearson();
};

void TestSimilarity::testpearson()
{
    unordered_map<int, unordered_map<int, tuple<double, long>>> ratings{};
    ratings[1000][2000] = make_tuple(1 , 12345678);
    ratings[1000][3000] = make_tuple(5 , 12345679);
    ratings[1000][4000] = make_tuple(10, 12345680);
    ratings[1000][5000] = make_tuple(3 , 12345681);

    ratings[2000][2000] = make_tuple(2 , 12345688);
    ratings[2000][3000] = make_tuple(6 , 12345689);
    ratings[2000][4000] = make_tuple(1 , 12345690);
    ratings[2000][6000] = make_tuple(3 , 12345682);

    cout << pearson(ratings[1000], ratings[2000]);
}



