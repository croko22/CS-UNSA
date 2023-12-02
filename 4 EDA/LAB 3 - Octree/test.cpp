#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCubeSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

int main(int, char *[])
{

    vtkNew<vtkNamedColors> colors;

    // Create a rendering window and renderer.
    vtkNew<vtkRenderer> ren;
    vtkNew<vtkRenderWindow> renWin;
    renWin->SetWindowName("Cube");
    renWin->AddRenderer(ren);
    // Create a renderwindow interactor.
    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    // Create a cube.
    vtkNew<vtkCubeSource> cube;
    cube->Update();
    cube->SetBounds(0, 1, 0, 1, 0, 1);

    // Mapper.
    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputData(cube->GetOutput());

    // Actor.
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper);
    cubeActor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
    cubeActor->SetPosition(10, 10, 10);

    // Actor 2
    vtkNew<vtkActor> cubeActor2;
    cubeActor2->SetMapper(cubeMapper);
    cubeActor2->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    cubeActor2->SetPosition(1, 1, 10);

    // Assign actor to the renderer.
    ren->AddActor(cubeActor);
    ren->AddActor(cubeActor2);

    ren->ResetCamera();
    ren->GetActiveCamera()->Azimuth(30);
    ren->GetActiveCamera()->Elevation(30);
    ren->ResetCameraClippingRange();
    ren->SetBackground(colors->GetColor3d("Silver").GetData());

    renWin->SetSize(300, 300);
    renWin->SetWindowName("Cube1");

    // Enable user interface interaction.
    iren->Initialize();
    renWin->Render();
    iren->Start();

    return EXIT_SUCCESS;
}