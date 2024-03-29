#include <bits/stdc++.h>
#include "kdtree.h"
#include "knn.h"
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
        double x, y, z;
        sscanf(line.c_str(), "%lf,%lf,%lf", &x, &y, &z);
        data.push_back(Point(x, y, z));
    }
    fin.close();
    return data;
}

std::tuple<Point, int, double> testKNN(std::vector<Point> &data, int k)
{
    Point query = data[rand() % data.size()];

    auto start = chrono::steady_clock::now();
    auto neighbors = knn(data, query, k);
    auto end = chrono::steady_clock::now();

    double time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    return std::make_tuple(query, neighbors.size(), time_ms);
}

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
    int option;
    cout << "Enter file name (1.1000, 2.10000, 3.20000):";
    cin >> option;
    string file = option == 1 ? "1000" : option == 2 ? "10000"
                                                     : "20000";
    auto data = loadPoints("data/" + file + ".csv");
    //* KNN
    const int k = 5;
    Point query(116, 6174, 1466);
    cout << "QUERY POINT: " << query[0] << " " << query[1] << " " << query[2] << endl;

    //* KNN Brute Force
    cout << "KNN Brute Force" << endl;
    auto start = chrono::steady_clock::now();
    vector<Point> ans = knn(data, query, k);
    auto end = chrono::steady_clock::now();
    for (auto &i : ans)
        cout << i[0] << " " << i[1] << " " << i[2] << endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    //*KD TREE
    cout << "KD TREE" << endl;
    KDTree<Point> kdtree(data);
    auto start1 = chrono::steady_clock::now();
    const std::vector<int> knnIndices = kdtree.knnSearch(query, k);
    auto end1 = chrono::steady_clock::now();
    for (auto i : knnIndices)
        cout << data[i][0] << ", " << data[i][1] << ", " << data[i][2] << std::endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end1 - start1).count() << "ms" << endl;

    //* MEASURE TIME
    int k_max = 1000;
    vector<tuple<Point, int, double>> knn_results;
    vector<tuple<Point, int, double>> kdtree_results;
    for (int i = 1; i <= k_max; i++)
    {
        auto knn_result = testKNN(data, i);
        auto kdtree_result = testKDTree(data, i);
        knn_results.push_back(knn_result);
        kdtree_results.push_back(kdtree_result);
    }
    ofstream fout("results/" + file + ".csv");

    fout << "k,knn_time, kdtree_time" << endl;
    for (int i = 0; i < k_max; i++)
    {
        fout << get<1>(knn_results[i]) << "," << get<2>(knn_results[i]) << "," << get<2>(kdtree_results[i]) << endl;
    }

    fout.close();

    return 0;
}