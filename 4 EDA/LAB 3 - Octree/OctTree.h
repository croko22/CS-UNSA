struct Point
{
    int x, y, z;
    Point(int x, int y, int z) : x(x), y(y), z(z) {}
};

class OctTree
{
private:
    OctTree *children[8];
    Point *points;

    // bottomLeft
    Point *bottomLeft;
    double h;

    int nPoints; // puntos en el cubo
public:
    OctTree();
    bool exist(const Point *p);
    void insert(const Point *p);
    ~OctTree();
};

OctTree::OctTree(/* args */) {}

OctTree::~OctTree() {}

void OctTree::insert(const Point *p)
{
}

bool OctTree::exist(const Point *p)
{
    return false;
}
