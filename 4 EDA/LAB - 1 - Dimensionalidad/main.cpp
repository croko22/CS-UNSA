#include <iostream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

vector<float> gRandomPoint(int d, float min, float max)
{
    vector<float> point;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);
    for (int i = 0; i < d; i++)
        point.push_back(dis(gen));
    return point;
}

vector<vector<float>> gRandomPoints(int n, int d, float min, float max)
{
    vector<vector<float>> points;
    for (int i = 0; i < n; i++)
        points.push_back(gRandomPoint(d, min, max));
    return points;
}

float euclideanDistance(vector<float> p1, vector<float> p2)
{
    float sum = 0;
    for (int i = 0; i < p1.size(); i++)
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    return sqrt(sum);
}

vector<float> distances(vector<vector<float>> points)
{
    vector<float> distances;
    //* Escribir las distancias entre todos los puntos en un csv
    fstream file("distances.csv", ios::out);
    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        for (int j = 0; j < points.size(); j++)
        {
            auto p = points[j];
            float dist = euclideanDistance(point, p);
            distances.push_back(dist);
            file << dist << ",";
        }
        file << endl;
    }
    return distances;
}

int main(int argc, char const *argv[])
{
    auto points = gRandomPoints(10, 3, 0, 1);
    // auto points = gRandomPoints(100, 3, 0, 1);
    for (auto point : points)
    {
        for (auto p : point)
            cout << p << " ";
        cout << endl;
    }

    auto dists = distances(points);
    for (auto dist : dists)
        cout << dist << " ";
    return 0;
}
