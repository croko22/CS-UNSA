#include <GL/glut.h>
#include <cmath>

class Carro
{
public:
    Carro(float width, float height) : width(width), height(height), translateX(0.0f), translateY(0.0f) {}

    void setTranslation(float x, float y)
    {
        translateX = x;
        translateY = y;
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        drawBody();
        drawRoof();
        drawWheels();
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

    void drawBody()
    {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        // Bottom
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(width / 2, -height / 2);
        // Top
        glVertex2f(-width / 2, height / 4);
        glVertex2f(width / 2, height / 4);
        // Left
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(-width / 2, height / 4);
        // Right
        glVertex2f(width / 2, -height / 2);
        glVertex2f(width / 2, height / 4);
        glEnd();
    }

    void drawRoof()
    {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        // Bottom left to top left
        glVertex2f(-width / 4, height / 4);
        glVertex2f(-width / 8, height / 2);
        // Bottom right to top right
        glVertex2f(width / 4, height / 4);
        glVertex2f(width / 8, height / 2);
        // Top left to top right
        glVertex2f(-width / 8, height / 2);
        glVertex2f(width / 8, height / 2);
        glEnd();
    }

    void drawWheels()
    {
        float wheelRadius = width / 8;
        drawWheel(-width / 3, -height / 2 - wheelRadius, wheelRadius);
        drawWheel(width / 3, -height / 2 - wheelRadius, wheelRadius);
    }

    void drawWheel(float cx, float cy, float r)
    {
        int num_segments = 100;
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
            float x = r * cosf(theta);
            float y = r * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    }
};
