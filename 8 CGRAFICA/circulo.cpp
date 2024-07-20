#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <vector>
using namespace std;

int centroX, centroY, radio;
vector<int> sectores;          // Vector para almacenar los porcentajes de los sectores
vector<vector<float>> colores; // Vector para almacenar los colores de los sectores

void dibujarPunto(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x + centroX, y + centroY);
    glEnd();
}

void dibujarLinea(int x1, int y1, int x2, int y2)
{
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void iniciar(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.5f, 1.0f, 0.0f);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1000.0, 1000.0, -1000.0, 1000.0);
}

void circuloPuntoMedio()
{
    int x = 0;
    int y = radio;
    float decision = 5 / 4 - radio;
    dibujarPunto(x, y);

    while (y > x)
    {
        if (decision < 0)
        {
            x++;
            decision += 2 * x + 1;
        }
        else
        {
            y--;
            x++;
            decision += 2 * (x - y) + 1;
        }
        dibujarPunto(x, y);
        dibujarPunto(x, -y);
        dibujarPunto(-x, y);
        dibujarPunto(-x, -y);
        dibujarPunto(y, x);
        dibujarPunto(-y, x);
        dibujarPunto(y, -x);
        dibujarPunto(-y, -x);
    }
}

void dibujarSectores()
{
    float anguloInicio = 0.0f;
    for (int i = 0; i < sectores.size(); ++i)
    {
        float angulo = (anguloInicio + sectores[i] * 3.6f) * (M_PI / 180.0f); // Convertir grados a radianes
        int x = centroX + radio * cos(angulo);
        int y = centroY + radio * sin(angulo);

        glColor3f(colores[i][0], colores[i][1], colores[i][2]); // Establecer el color para el sector
        glLineWidth(3.0);
        dibujarLinea(centroX, centroY, x, y);

        anguloInicio += sectores[i] * 3.6f; // Actualizar el ángulo de inicio
    }
}

void mostrar(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);

    circuloPuntoMedio();
    dibujarSectores();

    glFlush();
}

void generarColoresAleatorios()
{
    colores.resize(sectores.size());
    for (auto &color : colores)
    {
        color = {static_cast<float>(rand()) / RAND_MAX,
                 static_cast<float>(rand()) / RAND_MAX,
                 static_cast<float>(rand()) / RAND_MAX};
    }
}

int main(int argc, char **argv)
{
    centroX = 0, centroY = 0, radio = 500;
    // cout << "Arreglo de 3 elementos :";
    // sectores = {50, 25, 25};

    // cout << "Arreglo de 4 elementos :";
    // sectores = {5, 45, 35, 15};

    // cout << "Arreglo de 5 elementos :";
    // sectores = {15, 25, 30, 20, 10};

    cout << "Arreglo de 6 elementos :";
    sectores = {30, 20, 15, 10, 15, 10};

    generarColoresAleatorios();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Algoritmo de Círculo de Punto Medio");
    glutDisplayFunc(mostrar);
    iniciar();
    glutMainLoop();
}
