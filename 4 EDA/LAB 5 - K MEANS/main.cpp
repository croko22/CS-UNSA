#include <bits/stdc++.h>
#include "kdtree.h"
#include "kmeans.h"
using namespace std;

//* Load data
vector<Point> loadPoints(string filename)
{
    ifstream fin(filename);
    string line;
    vector<Point> data;
    if (!fin.is_open())
    {
        cout << "Error opening file" << endl;
        exit(1);
    }
    while (getline(fin, line))
    {
        double x, y;
        sscanf(line.c_str(), "%lf,%lf", &x, &y);
        data.push_back(Point(x, y));
    }
    fin.close();
    return data;
}

// std::tuple<Point, int, double> testKNN(std::vector<Point> &data, int k)
// {
//     Point query = data[rand() % data.size()];

//     auto start = chrono::steady_clock::now();
//     // auto neighbors = knn(data, query, k);
//     auto end = chrono::steady_clock::now();

//     double time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

//     // return std::make_tuple(query, neighbors.size(), time_ms);
// }

std::tuple<Point, int, double> testKDTree(std::vector<Point> &data, int k)
{
    Point query = data[rand() % data.size()];

    KDTree<Point> kdtree(data);
    auto start = chrono::steady_clock::now();
    const std::vector<int> knnIndices = kdtree.knnSearch(query, k);
    auto end = chrono::steady_clock::now();

    double time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    return std::make_tuple(query, knnIndices.size(), time_ms);
}

int main(int argc, char const *argv[])
{
    //* Load data
    auto data = loadPoints("data/data2k.csv");
    // for (auto &i : data)
    //     cout << i[0] << " " << i[1] << endl;

    const int k = 5;
    cout << "KMEANS" << endl;
    auto start0 = chrono::steady_clock::now();
    auto results = kmeans(data, k);
    auto end0 = chrono::steady_clock::now();
    for (auto &i : results)
        cout << i[0] << " " << i[1] << endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end0 - start0).count() << "ms" << endl;

    //*KD TREE
    cout << "KD TREE" << endl;
    KDTree<Point> kdtree(data);
    auto start1 = chrono::steady_clock::now();
    const std::vector<Point> kmeans = kdkmeans(data, k);
    auto end1 = chrono::steady_clock::now();
    for (auto i : kmeans)
        cout << i[0] << " " << i[1] << endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end1 - start1).count() << "ms" << endl;

    //* MEASURE TIME
    // int k_max = 1000;
    vector<tuple<Point, int, double>> knn_results;
    vector<tuple<Point, int, double>> kdtree_results;
    // for (int i = 1; i <= k_max; i++)
    // {
    //     auto knn_result = testKNN(data, i);
    //     auto kdtree_result = testKDTree(data, i);
    //     knn_results.push_back(knn_result);
    //     kdtree_results.push_back(kdtree_result);
    // }

    return 0;
}