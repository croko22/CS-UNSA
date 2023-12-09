#include <bits/stdc++.h>
using namespace std;

struct Point
{
    double x, y, z;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

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
    ifstream fin("data/1000.csv");
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

    vector<Point> ans = knn(data, data[0], 5);
    for (auto &i : ans)
        cout << i.x << " " << i.y << " " << i.z << endl;

    return 0;
}
