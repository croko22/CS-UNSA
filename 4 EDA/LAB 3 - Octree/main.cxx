#include <fstream>
#include <string>
#include "src/octree.h"
#include "src/visualizer.h"
using namespace std;

int main(int argc, char const *argv[])
{
  vtkNew<vtkNamedColors> colors;

  Octree *octree = new Octree();
  cout << "Insertando" << endl;
  std::ifstream file("assets/points1.csv");
  if (!file.is_open())
  {
    cout << "No se pudo abrir el archivo" << endl;
    return 1;
  }
  if (file.is_open())
  {
    std::string line;
    cout << "Leyendo" << endl;
    while (getline(file, line))
    {
      double x, y, z;
      sscanf(line.c_str(), "%lf,%lf,%lf", &x, &y, &z);
      Point p(x, y, z);
      octree->insert(p);
    }
  }

  Visualizer visualizer;
  visualizer.visualizeOctree(octree);
  visualizer.render();

  delete octree;

  return 0;
}