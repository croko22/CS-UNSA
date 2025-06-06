#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Camera control variables
static float s_zoom = 10.0f;
static float s_angleX = -20.0f;
static float s_angleY = 30.0f;
static int s_mouseX = 0;
static int s_mouseY = 0;
static bool s_mouseLeftDown = false;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Set camera position
    glTranslatef(0.0f, 0.0f, -s_zoom);

    // Apply rotations based on mouse movement
    glRotatef(s_angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(s_angleY, 0.0f, 1.0f, 0.0f);

    // Tamaño y separación entre esferas
    float sphereSize = 0.2f;
    float spacing = 0.5f;

    // Dibujamos 3x3x3 esferas
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            for (int z = 0; z < 3; ++z)
            {
                glPushMatrix();
                // Posicionamos cada esfera en la grilla 3D
                glTranslatef(
                    (x - 1) * spacing, // Centrado en X
                    (y - 1) * spacing, // Centrado en Y
                    (z - 1) * spacing  // Centrado en Z
                );
                glutSolidSphere(sphereSize, 30, 30);
                glPopMatrix();
            }
        }
    }

    glPopMatrix();
    glutSwapBuffers(); // Changed to double buffering for smoother interaction
}

void reshape(GLint w, GLint h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1f, 100.0f); // Changed to perspective projection
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            s_mouseLeftDown = true;
            s_mouseX = x;
            s_mouseY = y;
        }
        else if (state == GLUT_UP)
        {
            s_mouseLeftDown = false;
        }
    }
    else if (button == 3) // Mouse wheel up
    {
        s_zoom -= 0.5f;
        if (s_zoom < 1.0f)
            s_zoom = 1.0f;
        glutPostRedisplay();
    }
    else if (button == 4) // Mouse wheel down
    {
        s_zoom += 0.5f;
        if (s_zoom > 20.0f)
            s_zoom = 20.0f;
        glutPostRedisplay();
    }
}

void motion(int x, int y)
{
    if (s_mouseLeftDown)
    {
        s_angleY += (x - s_mouseX);
        s_angleX += (y - s_mouseY);
        s_mouseX = x;
        s_mouseY = y;

        glutPostRedisplay();
    }
}

void init()
{
    GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat yellow[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat cyan[] = {0.0, 1.0, 1.0, 1.0};
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat direction[] = {1.0, 1.0, 1.0, 0.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);

    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT0, GL_POSITION, direction);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Changed to double buffering
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Sphere Matrix with Camera Control");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    init();
    glutMainLoop();
    return 0;
}