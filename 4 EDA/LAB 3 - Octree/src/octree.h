struct Point
{
    double x, y, z;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Octree
{
private:
    struct Node
    {
        Point point;
        Node *children[8];

        Node(const Point &p) : point(p)
        {
            for (int i = 0; i < 8; ++i)
                children[i] = nullptr;
        }
    };

    Node *root;

public:
    Octree() : root(nullptr) {}
    void insert(const Point &p);
    bool exist(const Point &p);
    ~Octree();

private:
    void insertRecursively(const Point &p, Node *&node);
    bool existRecursively(const Point &p, Node *node);
};

void Octree::insert(const Point &p)
{
    insertRecursively(p, root);
}

bool Octree::exist(const Point &p)
{
    return existRecursively(p, root);
}

void Octree::insertRecursively(const Point &p, Node *&node)
{
    if (!node)
    {
        node = new Node(p);
        return;
    }

    // Determine the octant where the point should go
    int octant = (p.x < node->point.x ? 0 : 1) + (p.y < node->point.y ? 0 : 2) + (p.z < node->point.z ? 0 : 4);

    if (!node->children[octant])
        node->children[octant] = new Node(p);
    else
        insertRecursively(p, node->children[octant]);
}

bool Octree::existRecursively(const Point &p, Node *node)
{
    if (!node)
        return false;

    if (node->point.x == p.x && node->point.y == p.y && node->point.z == p.z)
        return true;

    int octant = (p.x < node->point.x ? 0 : 1) + (p.y < node->point.y ? 0 : 2) + (p.z < node->point.z ? 0 : 4);

    return existRecursively(p, node->children[octant]);
}

Octree::~Octree()
{
    delete root;
}
