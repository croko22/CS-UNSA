#pragma once
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
        double d = sqrt(pow(i[0] - p[0], 2) + pow(i[1] - p[1], 2) + pow(i[2] - p[2], 2));
        dist.push_back({d, i});
    }
    sort(dist.begin(), dist.end(), [](pair<double, Point> &a, pair<double, Point> &b)
         { return a.first < b.first; });
    vector<Point> ans;
    for (int i = 0; i < k; i++)
        ans.push_back(dist[i].second);
    return ans;
}
