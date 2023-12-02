#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <fstream>
#include <string>
#include "src/octree.h"
using namespace std;

void visualizeNode(Octree::Node *node, vtkRenderer *renderer)
{
  vtkNew<vtkNamedColors> colors;

  if (!node)
    return;

  vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
  cubeSource->SetBounds(
      static_cast<double>(node->point.x), static_cast<double>(node->point.x) + 10.0,
      static_cast<double>(node->point.y), static_cast<double>(node->point.y) + 10.0,
      static_cast<double>(node->point.z), static_cast<double>(node->point.z) + 10.0);

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(cubeSource->GetOutputPort());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

  renderer->AddActor(actor);

  for (int i = 0; i < 8; ++i)
  {
    visualizeNode(node->children[i], renderer);
  }
}

void visualizeOctree(Octree *octree, vtkRenderer *renderer)
{
  visualizeNode(octree->root, renderer);
}

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
  cout << "Insertados" << endl;

  vtkNew<vtkRenderer> ren;
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(ren);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  visualizeOctree(octree, ren);

  ren->ResetCamera();
  ren->GetActiveCamera()->Azimuth(30);
  ren->GetActiveCamera()->Elevation(30);
  ren->ResetCameraClippingRange();
  ren->SetBackground(colors->GetColor3d("Silver").GetData());

  renderWindow->SetSize(1200, 800);
  renderWindow->SetWindowName("OCTREE");

  renderWindowInteractor->Initialize();
  renderWindow->Render();
  renderWindowInteractor->Start();

  delete octree;

  return 0;
}