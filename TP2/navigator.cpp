#include <cstdlib>

// for mac osx
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
// only for windows
#ifdef _WIN32
#include <windows.h>
#endif
// for windows and linux
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <iostream>
#include <cmath>
using namespace std;

#define SPEED 0.1f //OpenGL unit
#define ANG_SPEED 0.05f //degrees

// variables containing the "fake" position and the camera and its angle
GLfloat x = .0f;
GLfloat y = .0f;
GLfloat z = 5.0f;

GLfloat theta = -M_PI_2;
GLfloat psi = 0;


void display()
{
    // clear window
    glClear(GL_COLOR_BUFFER_BIT);

    // set current matrix as GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);

    // draw scene
    glLoadIdentity();

    GLfloat lookAtX = x + cos(theta) * cos(psi);
    GLfloat lookAtY = y + sin(psi);
    GLfloat lookAtZ = z + sin(theta) * cos(psi);

    GLfloat upX = 0.0f ;
    GLfloat upY = 1.0f ;
    GLfloat upZ = 0.0f ;

    gluLookAt(x, y, z, lookAtX, lookAtY, lookAtZ -1. , upX, upY, upZ);

    // move the objects according to the "fake" camera position and angle
    // glTranslatef(x, y, z);
    // glRotatef(theta, 0.0f, 1.0f, 0.0f);
    // glRotatef(psi, 1.0f, 0.0f, 0.0f);
    // Doesn't work for now

    // add a copy of the curr. matrix to the stack
    glPushMatrix();

        glPushMatrix();

            // translate by -3 on the z
            glTranslatef(.0f, .0f, -3.f);
            // set drawing color to red
            glColor3f(1.0f, 0.0f, 0.0f);
            // middle red teapot
            glutWireTeapot(1);
            // translate by 2 on the y
            glTranslatef(.0f, 2.f, .0f);
            // set drawing color to green
            glColor3f(.0f, 1.0f, .0f);
            // rotate 90 deg around x
            glRotatef(90.f, 1.0f, 0.0f, 0.0f);
            // top green teapot
            glutWireTeapot(1);
        // pop the current matrix
        glPopMatrix();

        
        // translate -2 on y and -1 on z
        glTranslatef(.0f, -2.f, -1.f);
        // set drawing color to blue
        glColor3f(.0f, .0f, 1.f);
        // bottom blue teapot
        glutWireTeapot(1);

    // pop the current matrix
    glPopMatrix();

    // flush drawing routines to the window
    glFlush();
}

// Function called everytime a key is pressed
void key(unsigned char key, int, int)
{
    switch(key)
    {
        case 27: exit(EXIT_SUCCESS); break;
        case 'q': x -= SPEED; break;
        case 'd': x += SPEED; break;
        case 'z': z -= SPEED; break;
        case 's': z += SPEED; break;
        case 'a': y += SPEED; break;
        case 'w': y -= SPEED; break;
        default: break;
    }
    glutPostRedisplay();
}
void specialkeys(int key, int, int)
{
    switch(key)
    { 
        case GLUT_KEY_LEFT: theta -= ANG_SPEED; break;
        case GLUT_KEY_RIGHT: theta += ANG_SPEED; break;
        case GLUT_KEY_UP: psi += ANG_SPEED; break;
        case GLUT_KEY_DOWN: psi -= ANG_SPEED; break;
        default: break;
    }
    if (psi > M_PI_2) psi = M_PI_2;
    if (psi < -M_PI_2) psi = -M_PI_2;
    glutPostRedisplay();
}

void reshape(int width, int height)
{
    // define the viewport transformation
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
    // initialize GLUT, using any commandline parameters passed to the program
    glutInit(&argc, argv);

    // setup the size, position, and display mode for new windows
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB);

    // create and set up a window
    glutCreateWindow("hello, teapots!");
    // Set up the callback functions
    // for display
    glutDisplayFunc(display);
    // for the keyboard
    glutKeyboardFunc(key);
    glutSpecialFunc(specialkeys);
    // for reshaping
    glutReshapeFunc(reshape);

    // define the projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 10);

    // define the viewing transformation
    glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(x, y, z, .0, .0, -1., .0, 1., .0);

    // tell GLUT to wait for events
    glutMainLoop();
}
