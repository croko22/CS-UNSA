#include <fstream>
#include <string>
#include "src/octree.h"
#include "src/visualizer.h"
using namespace std;

void octreeOperations()
{
  Octree *octree = new Octree();

  cout << "Insertando puntos (0,0,0), (0,0,1), (1,2,3)" << endl;
  octree->insert(Point(0, 0, 0));
  octree->insert(Point(0, 0, 1));
  octree->insert(Point(1, 2, 3));

  cout << "Existe (0,0,0): " << octree->exist(Point(0, 0, 0)) << endl;
  cout << "Existe (0,0,1): " << octree->exist(Point(0, 0, 1)) << endl;
  cout << "Existe (1,2,3): " << octree->exist(Point(1, 2, 3)) << endl;
  cout << "Existe (1,2,4): " << octree->exist(Point(1, 2, 4)) << endl;

  delete octree;
}

void visualizeOctree(int option)
{
  vtkNew<vtkNamedColors> colors;

  Octree *octree = new Octree();
  std::ifstream file("assets/points" + std::to_string(option) + ".csv");
  cout << "Insertando puntos de assets/points" + std::to_string(option) + ".csv" << endl;
  if (!file.is_open())
  {
    cout << "No se pudo abrir el archivo" << endl;
    return;
  }
  if (file.is_open())
  {
    std::string line;
    while (getline(file, line))
    {
      double x, y, z;
      sscanf(line.c_str(), "%lf,%lf,%lf", &x, &y, &z);
      octree->insert(Point(x, y, z));
    }
  }

  int size = 1;
  cout << "Ingrese el tamaño de los cubos: ";
  cin >> size;
  Visualizer visualizer(option, size);
  visualizer.visualizeOctree(octree);
  visualizer.render();

  delete octree;
}

int main(int argc, char const *argv[])
{
  int option;
  cout << "1. Operaciones con Octree" << endl;
  cout << "2. Visualizar Octree" << endl;
  cout << "Ingrese una opción: ";
  cin >> option;

  if (option == 1)
    octreeOperations();
  else if (option == 2)
  {
    cout << "1. Gato" << endl;
    cout << "2. Dragon" << endl;
    cout << "Ingrese una opción: ";
    cin >> option;
    visualizeOctree(option);
  }
  else
    return 0;
}