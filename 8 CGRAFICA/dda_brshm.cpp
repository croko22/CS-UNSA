#include <GL/glut.h>
#include <cmath>
#include <string>

void drawLineDDA(int x1, int y1, int x2, int y2, const std::string &lineType)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = fmax(fabs(dx), fabs(dy));
    float x_inc = dx / steps;
    float y_inc = dy / steps;
    float x = x1;
    float y = y1;

    glBegin(GL_LINES);
    for (int i = 0; i <= steps; i++)
    {
        bool draw = false;
        if (lineType == "solid")
        {
            draw = true;
        }
        else if (lineType == "dotted")
        {
            draw = (i % 4 == 0);
        }
        else if (lineType == "dashed")
        {
            draw = (i % 10 < 5);
        }

        if (draw)
        {
            glVertex2i(round(x), round(y));
        }
        x += x_inc;
        y += y_inc;
    }
    glEnd();
}

void drawLineBresenham(int x1, int y1, int x2, int y2, const std::string &lineType)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int count = 0;

    glBegin(GL_LINES);
    while (1)
    {
        bool draw = false;
        if (lineType == "solid")
        {
            draw = true;
        }
        else if (lineType == "dotted")
        {
            draw = (count % 4 == 0);
        }
        else if (lineType == "dashed")
        {
            draw = (count % 10 < 5);
        }

        if (draw)
        {
            glVertex2i(x1, y1);
        }

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
        count++;
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0); // Color negro para las líneas

    // Dibuja líneas en el primer viewport (lado izquierdo)
    glViewport(0, 0, 250, 500);
    drawLineDDA(10, 20, 200, 220, "solid");
    drawLineDDA(10, 40, 200, 240, "dotted");
    drawLineDDA(10, 60, 200, 260, "dashed");

    // Dibuja líneas en el segundo viewport (lado derecho)
    glViewport(250, 0, 250, 500);
    drawLineBresenham(10, 20, 200, 220, "solid");
    drawLineBresenham(10, 40, 200, 240, "dotted");
    drawLineBresenham(10, 60, 200, 260, "dashed");

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fondo blanco
    glColor3f(0.0, 0.0, 0.0);         // Color negro para las líneas
    glLineWidth(2.0);                 // Grosor de las líneas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 250.0, 0.0, 500.0); // Definir el área de dibujo
}

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
