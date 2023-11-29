#include "OctTree.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    OctTree *tree = new OctTree();
    // Point *p = new Point(1, 2, 3);
    // tree->insert(p);

    fstream file;
    file.open("points1.csv", ios::in);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            int x, y, z;
            cout << line << endl; //* debug
            sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
            Point *p = new Point(x, y, z);
            tree->insert(p);
        }
    }

    cout << "Insertados" << endl;
    return 0;
}
