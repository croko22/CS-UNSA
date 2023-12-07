#include <iostream>
#include <vector>
#include <random>
#include <fstream>
using namespace std;

//? Generar puntos
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

//? Calcular distancias
float euclideanDistance(vector<float> p1, vector<float> p2)
{
    float sum = 0;
    for (int i = 0; i < p1.size(); i++)
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    return sqrt(sum);
}

vector<float> calculateDistances(vector<vector<float>> points)
{
    vector<float> distances;

    for (int i = 0; i < points.size(); i++)
    {
        auto point = points[i];
        for (int j = 0; j < points.size(); j++)
        {
            auto p = points[j];
            float dist = euclideanDistance(point, p);
            if (i != j)
                distances.push_back(dist);
        }
    }

    return distances;
}

void writeDistancesToFile(vector<float> distances, string filename)
{
    fstream file(filename, ios::out);
    for (int i = 0; i < distances.size(); i++)
        i == distances.size() - 1 ? file << distances[i] : file << distances[i] << ",";
}

void testByDimension(int d, string filename)
{
    auto points = gRandomPoints(100, d, 0, 1);
    // for (auto point : points)
    // {
    //     for (auto p : point)
    //         cout << p << " ";
    //     cout << endl;
    // }
    auto dists = calculateDistances(points);
    // for (auto dist : dists) cout << dist << " ";
    writeDistancesToFile(dists, filename);
}

int main(int argc, char const *argv[])
{
    // testByDimension(10, "d10_distances.csv");
    // testByDimension(50, "d50_distances.csv");
    // testByDimension(100, "d100_distances.csv");
    // testByDimension(500, "d500_distances.csv");
    // testByDimension(1000, "d1000_distances.csv");
    // testByDimension(2000, "d2000_distances.csv");
    // testByDimension(5000, "d5000_distances.csv");
    return 0;
}
