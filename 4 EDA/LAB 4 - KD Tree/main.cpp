#include <bits/stdc++.h>
#include "kdtree.h"
using namespace std;

struct KNNPoint
{
    double x, y, z;
    KNNPoint() {}
    KNNPoint(double x, double y, double z) : x(x), y(y), z(z) {}
};

//* KNN Brute Force
vector<KNNPoint> knn(vector<KNNPoint> &data, KNNPoint &p, int k)
{
    vector<pair<double, KNNPoint>> dist;
    for (auto &i : data)
    {
        //* Euclidean Distance
        double d = sqrt(pow(i.x - p.x, 2) + pow(i.y - p.y, 2) + pow(i.z - p.z, 2));
        dist.push_back({d, i});
    }
    sort(dist.begin(), dist.end(), [](pair<double, KNNPoint> &a, pair<double, KNNPoint> &b)
         { return a.first < b.first; });
    vector<KNNPoint> ans;
    for (int i = 0; i < k; i++)
        ans.push_back(dist[i].second);
    return ans;
}

//* KNN KD Tree

int main(int argc, char const *argv[])
{
    ifstream fin("data/20000.csv");
    string line;
    vector<KNNPoint> data;
    while (getline(fin, line))
    {
        double x, y, z;
        sscanf(line.c_str(), "%lf,%lf,%lf", &x, &y, &z);
        // cout << x << " " << y << " " << z << endl;
        data.push_back(KNNPoint(x, y, z));
    }
    fin.close();

    auto start = chrono::steady_clock::now();
    vector<KNNPoint> ans = knn(data, data[777], 5);
    auto end = chrono::steady_clock::now();
    for (auto &i : ans)
        cout << i.x << " " << i.y << " " << i.z << endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    //*KD TREE
    std::vector<Point> points;
    points.push_back({1, 1, 1});
    points.push_back({1, 10, 1});
    points.push_back({100, 1, 101});
    points.push_back({1, 111, 111});
    points.push_back({1, 11, 99});

    kdt::KDTree<Point> kdtree(points);

    // generate query (center of the space)
    // k-nearest neigbors search
    cout << "KD TREE" << endl;
    const Point query(100, 111, 100);
    const int k = 10;
    const std::vector<int> knnIndices = kdtree.knnSearch(query, k);
    for (auto i : knnIndices)
    {
        std::cout << points[i][0] << points[i][1] << points[i][2] << std::endl;
    }

    return 0;
}