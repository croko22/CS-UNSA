#pragma once
#include <bits/stdc++.h>
#include "kdtree.h"
using namespace std;

double distance(const Point &p1, const Point &p2)
{
    double sum = 0.0;
    for (int i = 0; i < Point::DIM; ++i)
    {
        double diff = p1[i] - p2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

std::pair<std::vector<Point>, std::vector<std::vector<Point>>> kmeans(const std::vector<Point> &data, int k)
{
    std::vector<Point> centroids(k);
    std::vector<std::vector<Point>> clusters(k);

    // Centroides
    for (int i = 0; i < k; ++i)
    {
        centroids[i] = data[i];
    }

    while (true)
    {
        for (const Point &point : data)
        {
            int closestCentroid = 0;
            double minDistance = std::numeric_limits<double>::max();
            for (int i = 0; i < k; ++i)
            {
                double currentDistance = distance(point, centroids[i]);
                if (currentDistance < minDistance)
                {
                    closestCentroid = i;
                    minDistance = currentDistance;
                }
            }
            clusters[closestCentroid].push_back(point);
        }

        // Calcular centroides
        std::vector<Point> newCentroids(k);
        for (int i = 0; i < k; ++i)
        {
            double sumX = 0, sumY = 0;
            for (const Point &point : clusters[i])
            {
                sumX += point[0];
                sumY += point[1];
            }
            newCentroids[i] = {sumX / clusters[i].size(), sumY / clusters[i].size()};
        }

        // Si no hay cambios, terminar
        bool centroidsChanged = false;
        for (int i = 0; i < k; ++i)
        {
            if (distance(centroids[i], newCentroids[i]) > 0)
            {
                centroidsChanged = true;
                break;
            }
        }
        if (!centroidsChanged)
        {
            break;
        }
        centroids = newCentroids;

        // Limpiar clusters
        for (auto &cluster : clusters)
        {
            cluster.clear();
        }
    }

    return {centroids, clusters};
}

std::pair<std::vector<Point>, std::vector<std::vector<Point>>> kdkmeans(const std::vector<Point> &data, int k)
{
    // 1. Choose k random points as initial centroids
    std::vector<Point> centroids(k);
    std::vector<std::vector<Point>> clusters(k);
    for (int i = 0; i < k; ++i)
    {
        centroids[i] = data[i];
    }

    int maxIterations = 10; // Set a maximum number of iterations

    for (int iteration = 0; iteration < maxIterations; ++iteration)
    {
        // Clear the clusters for the next iteration
        for (auto &cluster : clusters)
        {
            cluster.clear();
        }

        // 2. Create a KDTree with these centroids
        KDTree<Point> kdtree(centroids);

        // 3. For each point in the dataset, find the nearest centroid
        for (const Point &point : data)
        {
            int idx = kdtree.knnSearch(point, 1)[0];
            clusters[idx].push_back(point);
        }

        // 4. For each cluster, calculate the new centroid
        std::vector<Point> newCentroids(k);
        for (int i = 0; i < k; ++i)
        {
            if (clusters[i].empty())
            {
                // If the cluster is empty, skip the centroid calculation
                newCentroids[i] = centroids[i];
                continue;
            }

            Point centroid;
            for (const Point &point : clusters[i])
            {
                centroid[0] += point[0];
                centroid[1] += point[1];
            }
            centroid[0] /= clusters[i].size();
            centroid[1] /= clusters[i].size();
            newCentroids[i] = centroid;
        }

        // 5. Check if centroids have changed significantly
        double diff = 0;
        for (int i = 0; i < k; ++i)
        {
            diff += distance(centroids[i], newCentroids[i]);
        }

        centroids = newCentroids;

        // 6. Repeat until the centroids do not change significantly
        if (diff < 1e-5)
        {
            break;
        }

        // Limpiar clusters
        // for (auto &cluster : clusters)
        // {
        //     cluster.clear();
        // }
    }

    return {centroids, clusters};
}