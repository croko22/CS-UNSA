#include "src/octree.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    Octree *octree = new Octree();
    // Point *p = new Point(1, 2, 3);
    // octree->insert(p);

    fstream file;
    file.open("assets/points1.csv", ios::in);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            double x, y, z;
            sscanf(line.c_str(), "%lf,%lf,%lf", &x, &y, &z);
            Point p(x, y, z);
            octree->insert(p);
        }
    }
    octree->insert(Point(1, 2, 3));

    cout << "Insertados" << endl;
    cout << "Existen?" << endl;
    cout << octree->exist(Point(1, 2, 3)) << endl;
    cout << octree->exist(Point(1, 2, 3)) << endl;
    cout << octree->exist(Point(1, 2, 3)) << endl;
    cout << octree->exist(Point(-122, 177, -71)) << endl;
    cout << octree->exist(Point(266, 246, 19)) << endl;
    cout << octree->exist(Point(266, 246, 11)) << endl;
    return 0;
}
