#include <GL/glut.h>
#include <cmath>
class Casa
{
public:
    Casa(float width, float height) : width(width), height(height), translateX(0.0f), translateY(0.0f) {}

    void setTranslation(float x, float y)
    {
        translateX = x;
        translateY = y;
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        drawWalls();
        drawRoof();
        drawDoor();
        glPopMatrix();
    }

private:
    float width, height;
    float translateX, translateY;

    void drawLineDDA(float x1, float y1, float x2, float y2)
    {
        float dx = x2 - x1;
        float dy = y2 - y1;
        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        float xInc = dx / (float)steps;
        float yInc = dy / (float)steps;
        float x = x1;
        float y = y1;
        glBegin(GL_POINTS);
        for (int i = 0; i <= steps; i++)
        {
            glVertex2f(x, y);
            x += xInc;
            y += yInc;
        }
        glEnd();
    }

    void drawRoof()
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawLineDDA(-width / 2, height / 2, 0.0f, height / 2 + height / 3);
        drawLineDDA(0.0f, height / 2 + height / 3, width / 2, height / 2);
    }

    void drawWalls()
    {
        glColor3f(0.8f, 0.5f, 0.8f);
        glBegin(GL_LINES);
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(-width / 2, height / 2);
        glVertex2f(-width / 2, height / 2);
        glVertex2f(width / 2, height / 2);
        glVertex2f(width / 2, height / 2);
        glVertex2f(width / 2, -height / 2);
        glVertex2f(width / 2, -height / 2);
        glVertex2f(-width / 2, -height / 2);
        glEnd();
    }

    void drawDoor()
    {
        float doorWidth = width / 5;
        float doorHeight = height / 3;
        glColor3f(0.3f, 0.5f, 0.8f);
        glBegin(GL_LINES);
        glVertex2f(-doorWidth / 2, -height / 2);
        glVertex2f(-doorWidth / 2, -height / 2 + doorHeight);
        glVertex2f(-doorWidth / 2, -height / 2 + doorHeight);
        glVertex2f(doorWidth / 2, -height / 2 + doorHeight);
        glVertex2f(doorWidth / 2, -height / 2 + doorHeight);
        glVertex2f(doorWidth / 2, -height / 2);
        glEnd();
    }
};
