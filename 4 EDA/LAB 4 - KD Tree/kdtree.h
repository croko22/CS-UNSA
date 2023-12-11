#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>

struct Point
{
    double x, y, z;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

class KDTree
{
private:
    struct Node
    {
        Point point;
        Node *left;
        Node *right;
        Node(Point p) : point(p), left(nullptr), right(nullptr) {}
    };

    Node *root;
    void insertRecursive(Node *&node, Point &p, int depth);
    void kNNRecursive(Node *node, Point &queryPoint, int depth, int k, std::priority_queue<std::pair<double, Point>> &nearestNeighbors);

public:
    KDTree() : root(nullptr)
    {
    }
    ~KDTree();

    void insert(Point &p);
    std::vector<Point> kNN(Point &queryPoint, int k);
};

KDTree::~KDTree()
{
    // You can implement tree destruction logic here if needed.
}

void KDTree::insert(Point &p)
{
    // Helper function for recursive insertion
    insertRecursive(root, p, 0);
}

void KDTree::insertRecursive(Node *&node, Point &p, int depth)
{
    // If the current node is null, create a new node
    if (node == nullptr)
    {
        node = new Node(p);
        return;
    }

    // Determine the current dimension to split on (x, y, or z)
    int currentDimension = depth % 3; // Assuming 3D points

    // Compare the point based on the current dimension
    if (currentDimension == 0)
    {
        if (p.x < node->point.x)
            insertRecursive(node->left, p, depth + 1);
        else
            insertRecursive(node->right, p, depth + 1);
    }
    else if (currentDimension == 1)
    {
        if (p.y < node->point.y)
            insertRecursive(node->left, p, depth + 1);
        else
            insertRecursive(node->right, p, depth + 1);
    }
    else
    {
        if (p.z < node->point.z)
            insertRecursive(node->left, p, depth + 1);
        else
            insertRecursive(node->right, p, depth + 1);
    }
}

std::vector<Point> KDTree::kNN(Point &queryPoint, int k)
{
    std::priority_queue<std::pair<double, Point>> nearestNeighbors;

    kNNRecursive(root, queryPoint, 0, k, nearestNeighbors);

    // Extract the k nearest neighbors from the priority queue
    std::vector<Point> result;
    while (!nearestNeighbors.empty())
    {
        result.push_back(nearestNeighbors.top().second);
        nearestNeighbors.pop();
    }

    // The result is currently in reverse order, so reverse it
    std::reverse(result.begin(), result.end());

    return result;
}

void KDTree::kNNRecursive(Node *node, Point &queryPoint, int depth, int k, std::priority_queue<std::pair<double, Point>> &nearestNeighbors)
{
    if (node == nullptr)
        return;

    // Calculate the distance between the query point and the current node's point
    double distance = sqrt(pow(queryPoint.x - node->point.x, 2) + pow(queryPoint.y - node->point.y, 2) + pow(queryPoint.z - node->point.z, 2));

    // Insert the current point into the priority queue
    nearestNeighbors.push({distance, node->point});

    // Check if the priority queue size exceeds k, remove the farthest point
    if (nearestNeighbors.size() > k)
    {
        nearestNeighbors.pop();
    }

    // Determine the current dimension to split on (x, y, or z)
    int currentDimension = depth % 3; // Assuming 3D points

    // Recursively search the side of the splitting plane that contains the query point
    if (currentDimension == 0)
    {
        if (queryPoint.x < node->point.x)
            kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        else
            kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
    }
    else if (currentDimension == 1)
    {
        if (queryPoint.y < node->point.y)
            kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        else
            kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
    }
    else
    {
        if (queryPoint.z < node->point.z)
            kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        else
            kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
    }

    // Check if the other side of the splitting plane needs to be explored
    double nodePointValue = (currentDimension == 0) ? node->point.x : (currentDimension == 1) ? node->point.y
                                                                                              : node->point.z;
    double queryPointValue = (currentDimension == 0) ? queryPoint.x : (currentDimension == 1) ? queryPoint.y
                                                                                              : queryPoint.z;

    if (nearestNeighbors.size() < k || fabs(nodePointValue - queryPointValue) < nearestNeighbors.top().first)
    {
        if (currentDimension == 0)
        {
            if (queryPoint.x < node->point.x)
                kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
            else
                kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        }
        else if (currentDimension == 1)
        {
            if (queryPoint.y < node->point.y)
                kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
            else
                kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        }
        else
        {
            if (queryPoint.z < node->point.z)
                kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
            else
                kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        }
    }
}

/*
void KDTree::kNNRecursive(Node *node, Point &queryPoint, int depth, int k, std::priority_queue<std::pair<double, Point>> &nearestNeighbors)
{
    if (node == nullptr)
        return;

    // Calculate the distance between the query point and the current node's point
    double distance = sqrt(pow(queryPoint.x - node->point.x, 2) + pow(queryPoint.y - node->point.y, 2) + pow(queryPoint.z - node->point.z, 2));

    // Insert the current point into the priority queue
    nearestNeighbors.push({distance, node->point});

    // Check if the priority queue size exceeds k, remove the farthest point
    if (nearestNeighbors.size() > k)
    {
        nearestNeighbors.pop();
    }

    // Determine the current dimension to split on (x, y, or z)
    int currentDimension = depth % 3; // Assuming 3D points

    // Recursively search the side of the splitting plane that contains the query point
    if (currentDimension == 0)
    {
        if (queryPoint.x < node->point.x)
            kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        else
            kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
    }
    else if (currentDimension == 1)
    {
        if (queryPoint.y < node->point.y)
            kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        else
            kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
    }
    else
    {
        if (queryPoint.z < node->point.z)
            kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        else
            kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
    }

    // Check if the other side of the splitting plane needs to be explored
    if (nearestNeighbors.size() < k || fabs(node->point[currentDimension] - queryPoint[currentDimension]) < nearestNeighbors.top().first)
    {
        if (currentDimension == 0)
        {
            if (queryPoint.x < node->point.x)
                kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
            else
                kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        }
        else if (currentDimension == 1)
        {
            if (queryPoint.y < node->point.y)
                kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
            else
                kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        }
        else
        {
            if (queryPoint.z < node->point.z)
                kNNRecursive(node->right, queryPoint, depth + 1, k, nearestNeighbors);
            else
                kNNRecursive(node->left, queryPoint, depth + 1, k, nearestNeighbors);
        }
    }
}
*/