#include <GL/glut.h>
#include <math.h>
#include <cstdio>

// Función para trazar la línea usando DDA
void drawLineDDA(int x1, int y1, int x2, int y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = fmax(abs(dx), abs(dy));
    float x_inc = dx / steps;
    float y_inc = dy / steps;
    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2i(round(x), round(y));
        x += x_inc;
        y += y_inc;
    }
    glEnd();
}

// Función para trazar la línea usando el algoritmo de Bresenham
void drawLineBresenham(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (1)
    {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
    glEnd();
}

// Función para dibujar la cuadrícula
void drawGrid()
{
    glColor3f(0.8, 0.8, 0.8); // Color gris para la cuadrícula
    glBegin(GL_LINES);
    for (int i = 0; i <= 20; i++)
    {
        glVertex2i(i, 0);
        glVertex2i(i, 20);
        glVertex2i(0, i);
        glVertex2i(20, i);
    }
    glEnd();

    // Dibujar las coordenadas
    glColor3f(0.0, 0.0, 0.0); // Color negro para las coordenadas
    for (int i = 0; i <= 20; i++)
    {
        glRasterPos2i(i, -1);
        char num[3];
        snprintf(num, 3, "%d", i);
        for (char *c = num; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
        glRasterPos2i(-1, i);
        snprintf(num, 3, "%d", i);
        for (char *c = num; *c != '\0'; c++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
    }
}

// Función de visualización
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibuja la cuadrícula en dos regiones
    glViewport(0, 0, 250, 500);
    drawGrid();

    glViewport(250, 0, 250, 500);
    drawGrid();

    // Dibuja la línea usando DDA en la primera región
    glViewport(0, 0, 250, 500);
    glColor3f(0.0, 0.0, 1.0); // Color azul
    drawLineDDA(2, 2, 10, 5);

    // Dibuja la línea usando Bresenham en la segunda región
    glViewport(250, 0, 250, 500);
    glColor3f(1.0, 0.0, 0.0); // Color rojo
    drawLineBresenham(2, 2, 10, 5);

    glFlush();
}

// Función de inicialización
void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fondo blanco
    glColor3f(0.0, 0.0, 0.0);         // Color negro para los puntos
    glPointSize(5.0);                 // Tamaño de los puntos
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 20.0, 0.0, 20.0); // Definir el área de dibujo
}

// Función principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Comparación de DDA y Bresenham en dos planos");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
