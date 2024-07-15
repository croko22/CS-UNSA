#include <GL/glut.h>
#include <iostream>
#include <thread>
#include <atomic>

class Casa
{
public:
    Casa(float width, float height) : width(width), height(height), scale(1.0f), translateX(0.0f), translateY(0.0f) {}

    void setTranslation(float x, float y)
    {
        translateX = x;
        translateY = y;
    }

    void setScale(float s)
    {
        scale = s;
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        glScalef(scale, scale, 1.0f);
        drawWalls();
        drawRoof();
        drawDoor();
        glPopMatrix();
    }

private:
    float width, height;
    float translateX, translateY, scale;

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

Casa *house;
std::atomic<bool> running(true);

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    house->draw();
    glFlush();
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(2.0);
    glLineWidth(3.0);
    gluOrtho2D(-1000.0, 1000.0, -1000.0, 1000.0);
}

void inputThread()
{
    std::string command;
    while (running)
    {
        std::cout << "Enter command (translate x y / scale s / exit): ";
        if (!(std::cin >> command))
        {
            std::cerr << "Error reading command. Please try again.\n";
            std::cin.clear();                                                   // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the rest of the line
            continue;
        }

        if (command == "translate")
        {
            float x, y;
            if (!(std::cin >> x >> y))
            {
                std::cerr << "Invalid input for translate. Please enter two numbers.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            if (house)
            {
                house->setTranslation(x, y);
                glutPostRedisplay();
            }
        }
        else if (command == "scale")
        {
            float s;
            if (!(std::cin >> s))
            {
                std::cerr << "Invalid input for scale. Please enter a number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            if (house)
            {
                house->setScale(s);
                glutPostRedisplay();
            }
        }
        else if (command == "exit")
        {
            running = false;
        }
        else
        {
            std::cerr << "Unknown command. Please try again.\n";
        }
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Interactive House with DDA Lines");
    init();

    house = new Casa(500, 200);

    std::thread input(inputThread);

    glutDisplayFunc(display);
    glutMainLoop();

    running = false;
    input.join();
    delete house;
    return 0;
}
