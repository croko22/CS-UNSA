#include <GL/glut.h>
#include <cmath>
#include <string>

void lineaDDA(int x1, int y1, int x2, int y2, const std::string &lineType)
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
        if (lineType == "solida")
            draw = true;
        else if (lineType == "punteada")
            draw = (i % 4 == 0);
        else if (lineType == "rayada")
            draw = (i % 10 < 5);

        if (draw)
            glVertex2i(round(x), round(y));
        x += x_inc;
        y += y_inc;
    }
    glEnd();
}

void lineaBresenham(int x1, int y1, int x2, int y2, const std::string &lineType)
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
        if (lineType == "solida")
            draw = true;
        else if (lineType == "punteada")
            draw = (count % 4 == 0);
        else if (lineType == "rayada")
            draw = (count % 10 < 5);

        if (draw)
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
        count++;
    }
    glEnd();
}

void drawGrid()
{
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_LINES);
    for (int i = 0; i <= 500; i += 20)
    {
        glVertex2i(i, 0);
        glVertex2i(i, 500);
        glVertex2i(0, i);
        glVertex2i(500, i);
    }
    glEnd();
}

void renderBitmapString(float x, float y, void *font, const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid on the entire screen
    glViewport(0, 0, 500, 500);
    drawGrid();

    // First viewport (left side): DDA lines and label
    glViewport(0, 0, 250, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 250.0, 0.0, 500.0);
    lineaDDA(10, 20, 240, 480, "solida");
    lineaDDA(10, 40, 240, 460, "punteada");
    lineaDDA(10, 60, 240, 440, "rayada");
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 10, GLUT_BITMAP_HELVETICA_18, "DDA Algorithm");

    // Second viewport (right side): Bresenham lines, zoomed in and label
    glViewport(250, 0, 250, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 250.0, 0.0, 500.0);
    glLineWidth(3.0); // Increase line width for zoomed view
    lineaBresenham(10, 20, 240, 480, "solida");
    lineaBresenham(10, 40, 240, 460, "punteada");
    lineaBresenham(10, 60, 240, 440, "rayada");
    glColor3f(0.0, 0.0, 0.0);
    renderBitmapString(50, 10, GLUT_BITMAP_HELVETICA_18, "Bresenham Algorithm");

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA y Bresenham");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
