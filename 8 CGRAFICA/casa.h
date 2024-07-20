#include <GL/glut.h>
#include <cmath>

class Casa
{
public:
    Casa(float ancho, float alto, float x, float y) : ancho(ancho), alto(alto), posX(x), posY(y) {}

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
        dibujarParedes();
        dibujarTecho();
        dibujarPuerta();
        glPopMatrix();
    }

private:
    float ancho, alto;
    float posX, posY;
    float escala = 1.0f;

    void dibujarLineaDDA(float x1, float y1, float x2, float y2)
    {
        float dx = x2 - x1;
        float dy = y2 - y1;
        int pasos = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        float xInc = dx / (float)pasos;
        float yInc = dy / (float)pasos;
        float x = x1;
        float y = y1;
        glBegin(GL_POINTS);
        for (int i = 0; i <= pasos; i++)
        {
            glVertex2f(x, y);
            x += xInc;
            y += yInc;
        }
        glEnd();
    }

    void dibujarTecho()
    {
        glColor3f(1.0f, 0.7f, 0.0f);
        dibujarLineaDDA(-ancho / 2, alto / 2, 0.0f, alto / 2 + alto / 3);
        dibujarLineaDDA(0.0f, alto / 2 + alto / 3, ancho / 2, alto / 2);
    }

    void dibujarParedes()
    {
        glColor3f(0.5f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(-ancho / 2, -alto / 2);
        glVertex2f(-ancho / 2, alto / 2);
        glVertex2f(-ancho / 2, alto / 2);
        glVertex2f(ancho / 2, alto / 2);
        glVertex2f(ancho / 2, alto / 2);
        glVertex2f(ancho / 2, -alto / 2);
        glVertex2f(ancho / 2, -alto / 2);
        glVertex2f(-ancho / 2, -alto / 2);
        glEnd();
    }

    void dibujarPuerta()
    {
        float anchoPuerta = ancho / 5;
        float altoPuerta = alto / 3;
        glColor3f(0.3f, 0.5f, 0.8f); // Azul para la puerta
        glBegin(GL_LINES);
        glVertex2f(-anchoPuerta / 2, -alto / 2);
        glVertex2f(-anchoPuerta / 2, -alto / 2 + altoPuerta);
        glVertex2f(-anchoPuerta / 2, -alto / 2 + altoPuerta);
        glVertex2f(anchoPuerta / 2, -alto / 2 + altoPuerta);
        glVertex2f(anchoPuerta / 2, -alto / 2 + altoPuerta);
        glVertex2f(anchoPuerta / 2, -alto / 2);
        glEnd();
    }
};
