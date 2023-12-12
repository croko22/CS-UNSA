#pragma once

class Point : public std::array<double, 3>
{
public:
    static const int DIM = 3;

    Point() {}
    Point(double x, double y, double z)
    {
        (*this)[0] = x;
        (*this)[1] = y;
        (*this)[2] = z;
    }
};

template <class PointT>
class KDTree
{
public:
    KDTree() : root_(nullptr){};
    KDTree(const std::vector<PointT> &points) : root_(nullptr) { build(points); }
    ~KDTree() { clear(); }

    void build(const std::vector<PointT> &points)
    {
        clear();

        points_ = points;

        std::vector<int> indices(points.size());
        std::iota(std::begin(indices), std::end(indices), 0);

        root_ = buildRecursive(indices.data(), (int)points.size(), 0);
    }

    void clear()
    {
        clearRecursive(root_);
        root_ = nullptr;
        points_.clear();
    }

    void insert(const PointT &point)
    {
        if (root_ == nullptr)
        {
            root_ = new Node();
            root_->idx = static_cast<int>(points_.size());
            points_.push_back(point);
        }
        else
            insertRecursive(point, root_, 0);
    }

    std::vector<int> knnSearch(const PointT &query, int k) const
    {
        KnnQueue queue(k);
        knnSearchRecursive(query, root_, queue, k);

        std::vector<int> indices(queue.size());
        for (size_t i = 0; i < queue.size(); i++)
            indices[i] = queue[i].second;

        return indices;
    }

private:
    struct Node
    {
        int idx;       //? index to the original point
        Node *next[2]; //? pointers to the child nodes
        int axis;      //? dimension's axis

        Node() : idx(-1), axis(-1) { next[0] = next[1] = nullptr; }
    };

    template <class T, class Compare = std::less<T>>
    class BoundedPriorityQueue
    {
    public:
        BoundedPriorityQueue() = delete;
        BoundedPriorityQueue(size_t bound) : bound_(bound) { elements_.reserve(bound + 1); };

        void push(const T &val)
        {
            auto it = std::find_if(std::begin(elements_), std::end(elements_),
                                   [&](const T &element)
                                   { return Compare()(val, element); });
            elements_.insert(it, val);

            if (elements_.size() > bound_)
                elements_.resize(bound_);
        }

        const T &back() const { return elements_.back(); };
        const T &operator[](size_t index) const { return elements_[index]; }
        size_t size() const { return elements_.size(); }

    private:
        size_t bound_;
        std::vector<T> elements_;
    };

    using KnnQueue = BoundedPriorityQueue<std::pair<double, int>>;

    Node *buildRecursive(int *indices, int npoints, int depth)
    {
        if (npoints <= 0)
            return nullptr;

        const int axis = depth % PointT::DIM;
        const int mid = (npoints - 1) / 2;

        std::nth_element(indices, indices + mid, indices + npoints, [&](int lhs, int rhs)
                         { return points_[lhs][axis] < points_[rhs][axis]; });

        Node *node = new Node();
        node->idx = indices[mid];
        node->axis = axis;

        node->next[0] = buildRecursive(indices, mid, depth + 1);
        node->next[1] = buildRecursive(indices + mid + 1, npoints - mid - 1, depth + 1);

        return node;
    }

    void clearRecursive(Node *node)
    {
        if (node == nullptr)
            return;

        if (node->next[0])
            clearRecursive(node->next[0]);

        if (node->next[1])
            clearRecursive(node->next[1]);

        delete node;
    }

    void insertRecursive(const PointT &point, Node *node, int depth)
    {
        const int axis = depth % PointT::DIM;

        if (point[axis] < points_[node->idx][axis])
        {
            if (node->next[0] == nullptr)
            {
                node->next[0] = new Node();
                node->next[0]->idx = static_cast<int>(points_.size());
                points_.push_back(point);
            }
            else
            {
                insertRecursive(point, node->next[0], depth + 1);
            }
        }
        else
        {
            if (node->next[1] == nullptr)
            {
                node->next[1] = new Node();
                node->next[1]->idx = static_cast<int>(points_.size());
                points_.push_back(point);
            }
            else
            {
                insertRecursive(point, node->next[1], depth + 1);
            }
        }
    }

    static double distance(const PointT &p, const PointT &q)
    {
        double dist = 0;
        for (size_t i = 0; i < PointT::DIM; i++)
            dist += (p[i] - q[i]) * (p[i] - q[i]);
        return sqrt(dist);
    }

    void knnSearchRecursive(const PointT &query, const Node *node, KnnQueue &queue, int k) const
    {
        if (node == nullptr)
            return;

        const PointT &train = points_[node->idx];

        const double dist = distance(query, train);
        queue.push(std::make_pair(dist, node->idx));

        const int axis = node->axis;
        const int dir = query[axis] < train[axis] ? 0 : 1;
        knnSearchRecursive(query, node->next[dir], queue, k);

        const double diff = fabs(query[axis] - train[axis]);
        if ((int)queue.size() < k || diff < queue.back().first)
            knnSearchRecursive(query, node->next[!dir], queue, k);
    }

    Node *root_;                 //? root node
    std::vector<PointT> points_; //? points
};
