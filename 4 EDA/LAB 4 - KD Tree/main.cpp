#include <bits/stdc++.h>
#include "kdtree.h"
using namespace std;

//* KNN Brute Force
vector<Point> knn(vector<Point> &data, Point &p, int k)
{
    vector<pair<double, Point>> dist;
    for (auto &i : data)
    {
        //* Euclidean Distance
        double d = sqrt(pow(i.x - p.x, 2) + pow(i.y - p.y, 2) + pow(i.z - p.z, 2));
        dist.push_back({d, i});
    }
    sort(dist.begin(), dist.end(), [](pair<double, Point> &a, pair<double, Point> &b)
         { return a.first < b.first; });
    vector<Point> ans;
    for (int i = 0; i < k; i++)
        ans.push_back(dist[i].second);
    return ans;
}

//* KNN KD Tree

int main(int argc, char const *argv[])
{
    ifstream fin("data/20000.csv");
    string line;
    vector<Point> data;
    while (getline(fin, line))
    {
        double x, y, z;
        sscanf(line.c_str(), "%lf,%lf,%lf", &x, &y, &z);
        // cout << x << " " << y << " " << z << endl;
        data.push_back(Point(x, y, z));
    }
    fin.close();

    auto start = chrono::steady_clock::now();
    vector<Point> ans = knn(data, data[777], 5);
    auto end = chrono::steady_clock::now();
    for (auto &i : ans)
        cout << i.x << " " << i.y << " " << i.z << endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    //*KD TREE
    KDTree kdTree;
    Point p1(1.0, 2.0, 3.0);
    Point p2(4.0, 5.0, 6.0);
    Point p3(7.0, 8.0, 9.0);

    kdTree.insert(p1);
    kdTree.insert(p2);
    kdTree.insert(p3);

    Point queryPoint(0.0, 0.0, 0.0);
    int k = 2;

    std::vector<Point> nearestNeighbors = kdTree.kNN(queryPoint, k);

    // Display the k nearest neighbors
    for (const auto &neighbor : nearestNeighbors)
    {
        std::cout << "Point (" << neighbor.x << ", " << neighbor.y << ", " << neighbor.z << ")" << std::endl;
    }

    return 0;
}
