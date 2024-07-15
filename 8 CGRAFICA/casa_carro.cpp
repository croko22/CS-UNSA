#include <GL/glut.h>
#include <cmath>
#include "casa.h"
#include "carro.h"
Casa *house;
Carro *car;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    house->draw();
    car->draw();
    glFlush();
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(2.0);
    glLineWidth(3.0);
    gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("House with DDA Lines");
    init();

    house = new Casa(500, 400);
    house->setTranslation(500.0f, 500.0f);

    car = new Carro(600, 200);
    car->setTranslation(250.0f, 250.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    delete house;
    delete car;
    return 0;
}
