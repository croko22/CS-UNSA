#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <vector>
using namespace std;

int pntX1, pntY1, r;
vector<int> sectors; // Vector to hold sector percentages

void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x + pntX1, y + pntY1);
    glEnd();
}

void drawLine(int x1, int y1, int x2, int y2)
{
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void myInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1000.0, 1000.0, -1000.0, 1000.0);
}

void midPointCircle()
{
    int x = 0;
    int y = r;
    float decision = 5 / 4 - r;
    plot(x, y);

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
        plot(x, y);
        plot(x, -y);
        plot(-x, y);
        plot(-x, -y);
        plot(y, x);
        plot(-y, x);
        plot(y, -x);
        plot(-y, -x);
    }
}

void drawSectors()
{
    float startAngle = 0.0f;
    for (int i = 0; i < sectors.size(); ++i)
    {
        float angle = (startAngle + sectors[i] * 3.6f) * (M_PI / 180.0f); // Convert degrees to radians
        int x = pntX1 + r * cos(angle);
        int y = pntY1 + r * sin(angle);
        drawLine(pntX1, pntY1, x, y);
        startAngle += sectors[i] * 3.6f; // Update start angle
    }
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);

    midPointCircle();
    drawSectors();

    glFlush();
}

int main(int argc, char **argv)
{
    // cout << "Enter the coordinates of the center and radius:\n";
    // cout << "x y r: ";
    // cin >> pntX1 >> pntY1 >> r;

    // int numSectors;
    // cout << "Enter the number of sectors: ";
    // cin >> numSectors;
    // sectors.resize(numSectors);

    // cout << "Enter the sizes of the sectors (in percentage, summing to 100):\n";
    // for (int i = 0; i < numSectors; ++i)
    //     cin >> sectors[i];

    pntX1 = 0, pntY1 = 0, r = 450;
    sectors = {10, 7, 13, 5, 13, 14, 3, 16, 5, 3, 17, 8};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Line Drawing Algorithms");
    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();
}
