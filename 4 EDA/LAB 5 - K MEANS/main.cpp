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

int main(int argc, char const *argv[])
{
    //* Load data
    auto data = loadPoints("data/data2k.csv");
    // for (auto &i : data)
    //     cout << i[0] << " " << i[1] << endl;

    const int k = 18;
    //*KMEANS
    // auto results = kmeans(data, k);
    // fstream fout("data/kmeans/centroids.csv", ios::out | ios::app);
    // fout << "x,y" << std::endl;
    // for (auto i : results.first)
    //     fout << i[0] << "," << i[1] << std::endl;
    // fout.close();
    // for (int i = 0; i < k; ++i)
    // {
    //     fstream fout("data/kmeans/cluster_" + to_string(i) + ".csv", ios::out | ios::app);
    //     fout << "x,y" << std::endl;
    //     for (auto j : results.second[i])
    //         fout << j[0] << "," << j[1] << std::endl;
    //     fout.close();
    // }

    //* KDTREE
    // auto results = kdkmeans(data, k);
    // fstream fout("data/kdtree/centroids.csv", ios::out | ios::app);
    // fout << "x,y" << std::endl;
    // for (auto i : results.first)
    //     fout << i[0] << "," << i[1] << std::endl;
    // cout << "Clusters: " << endl;
    // for (int i = 0; i < k; ++i)
    // {
    //     cout << "Cluster " << i << ": " << results.second[i].size() << endl;
    // }
    // fout.close();
    // for (int i = 0; i < k; ++i)
    // {
    //     fstream fout("data/kdtree/cluster_" + to_string(i) + ".csv", ios::out | ios::app);
    //     fout << "x,y" << std::endl;
    //     for (auto j : results.second[i])
    //         fout << j[0] << "," << j[1] << std::endl;
    //     fout.close();
    // }

    int num_iterations = 10; // Number of iterations

    //*KMEANS
    for (int iteration = 0; iteration < num_iterations; ++iteration)
    {
        auto results = kmeans(data, k);

        // Save centroids
        std::fstream centroids_out("data/kmeans10/centroids_" + std::to_string(iteration) + ".csv", std::ios::out);
        centroids_out << "x,y" << std::endl;
        for (auto i : results.first)
            centroids_out << i[0] << "," << i[1] << std::endl;
        centroids_out.close();

        // Save clusters
        for (int i = 0; i < k; ++i)
        {
            std::fstream cluster_out("data/kmeans10/cluster_" + std::to_string(iteration) + "_" + std::to_string(i) + ".csv", std::ios::out);
            cluster_out << "x,y" << std::endl;
            for (auto j : results.second[i])
                cluster_out << j[0] << "," << j[1] << std::endl;
            cluster_out.close();
        }
    }

    // * KDTREE
    for (int iteration = 0; iteration < num_iterations; ++iteration)
    {
        auto results = kdkmeans(data, k);

        // Save centroids
        std::fstream centroids_out("data/kdtree10/centroids_" + std::to_string(iteration) + ".csv", std::ios::out);
        centroids_out << "x,y" << std::endl;
        for (auto i : results.first)
            centroids_out << i[0] << "," << i[1] << std::endl;
        centroids_out.close();

        // Save clusters
        for (int i = 0; i < k; ++i)
        {
            std::fstream cluster_out("data/kdtree10/cluster_" + std::to_string(iteration) + "_" + std::to_string(i) + ".csv", std::ios::out);
            cluster_out << "x,y" << std::endl;
            for (auto j : results.second[i])
                cluster_out << j[0] << "," << j[1] << std::endl;
            cluster_out.close();
        }
    }

    //* TESTS WITH K
    // std::vector<int> k_values = {5, 15, 25, 50, 75, 100, 125, 150, 200};
    // std::vector<int> n_values = {1000, 1450, 1900, 2400};

    // for (int k : k_values)
    // {
    //     fstream fout("data/k/" + to_string(k) + ".csv", ios::out | ios::app);
    //     fout << "n,kmeans_time,kdkmeans_time" << std::endl;
    //     for (int n : n_values)
    //     {
    //         // Get the first n points of the dataset
    //         std::vector<Point> data_subset(data.begin(), data.begin() + n);

    //         // Measure the execution time of the standard k-means
    //         auto start1 = std::chrono::high_resolution_clock::now();
    //         auto knn_result = kmeans(data_subset, k);
    //         auto end1 = std::chrono::high_resolution_clock::now();
    //         auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();

    //         // Measure the execution time of the KDTree-based k-means
    //         auto start2 = std::chrono::high_resolution_clock::now();
    //         auto kdtree_result = kdkmeans(data_subset, k);
    //         auto end2 = std::chrono::high_resolution_clock::now();
    //         auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();

    //         fout << n << "," << time1 << "," << time2 << std::endl;
    //     }
    // }

    //* TESTS WITH N
    // std::vector<int> k_values = {5, 15, 25, 50, 75};
    // std::vector<int> n_values = {1000, 1150, 1300, 1450, 1600, 1750, 1900, 2050, 2200, 2400};

    // for (int n : n_values)
    // {
    //     fstream fout("data/n/" + to_string(n) + ".csv", ios::out | ios::app);
    //     fout << "n,kmeans_time,kdkmeans_time" << std::endl;
    //     for (int k : k_values)
    //     {
    //         // Get the first n points of the dataset
    //         std::vector<Point> data_subset(data.begin(), data.begin() + n);

    //         // Measure the execution time of the standard k-means
    //         auto start1 = std::chrono::high_resolution_clock::now();
    //         auto knn_result = kmeans(data_subset, k);
    //         auto end1 = std::chrono::high_resolution_clock::now();
    //         auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();

    //         // Measure the execution time of the KDTree-based k-means
    //         auto start2 = std::chrono::high_resolution_clock::now();
    //         auto kdtree_result = kdkmeans(data_subset, k);
    //         auto end2 = std::chrono::high_resolution_clock::now();
    //         auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
    //         fout << k << "," << time1 << "," << time2 << std::endl;
    //     }
    // }

    return 0;
}