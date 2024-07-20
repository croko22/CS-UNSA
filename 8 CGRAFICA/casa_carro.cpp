#include <GL/glut.h>
#include <cmath>
#include "casa.h"
#include "carro.h"
Casa *house;
Carro *car;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    house->dibujar();
    car->dibujar();
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
    glutCreateWindow("CASA CARRO");
    init();

    house = new Casa(700, 350, 500, 500);
    house->escalar(0.3);
    car = new Carro(200, 100, 700, 300);
    car->escalar(2);
    car->traslacion(700, 300);

    glutDisplayFunc(display);
    glutMainLoop();
    delete house;
    delete car;
    return 0;
}
