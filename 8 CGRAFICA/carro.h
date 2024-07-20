#include <GL/glut.h>
#include <cmath>

class Carro
{
public:
    Carro(float width, float height, float posX, float posY) : width(width), height(height), posX(posX), posY(posY) {}

    void traslacion(float x, float y)
    {
        posX = x;
        posY = y;
    }

    void escalar(float s)
    {
        escala = s;
    }

    void dibujar()
    {
        glPushMatrix();
        glTranslatef(posX, posY, 0.0f);
        glScalef(escala, escala, 1.0f);
        dibujarCuerpo();
        dibujarTecho();
        dibujarRuedas();
        glPopMatrix();
    }

private:
    float width, height;
    float posX, posY;
    float escala = 1.0f;

    void dibujarLineaDDA(float x1, float y1, float x2, float y2)
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

    void dibujarCuerpo()
    {
        glColor3f(1.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        // Parte inferior
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(width / 2, -height / 2);
        // Parte superior
        glVertex2f(-width / 2, height / 4);
        glVertex2f(width / 2, height / 4);
        // Izquierda
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(-width / 2, height / 4);
        // Derecha
        glVertex2f(width / 2, -height / 2);
        glVertex2f(width / 2, height / 4);
        glEnd();
    }

    void dibujarTecho()
    {
        glColor3f(0.5f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        // Inferior izquierda a superior izquierda
        glVertex2f(-width / 4, height / 4);
        glVertex2f(-width / 8, height / 2);
        // Inferior derecha a superior derecha
        glVertex2f(width / 4, height / 4);
        glVertex2f(width / 8, height / 2);
        // Superior izquierda a superior derecha
        glVertex2f(-width / 8, height / 2);
        glVertex2f(width / 8, height / 2);
        glEnd();
    }

    void dibujarRuedas()
    {
        float radioRueda = width / 8;
        dibujarRueda(-width / 3, -height / 2 - radioRueda, radioRueda);
        dibujarRueda(width / 3, -height / 2 - radioRueda, radioRueda);
    }

    void dibujarRueda(float cx, float cy, float r)
    {
        int num_segments = 100;
        glColor3f(0.0f, 0.0f, 0.0f); // Negro para las ruedas
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
            float x = r * cosf(theta);
            float y = r * sinf(theta) + r;
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    }
};
