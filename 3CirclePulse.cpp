#include <GL/glut.h>
#include <cmath>

float radius = 0.05f;
bool expanding = true;

void drawCircle(float r) {
    if (r <= 0.0f) return;

    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    int segments = 100;
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = r * cos(theta);
        float y = r * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void update(int value) {
    if (expanding) {
        radius += 0.01f;
        if (radius >= 0.5f) expanding = false;
    }
    else {
        radius -= 0.015f; 
        if (radius <= 0.0f) {
            radius = 0.0f;
            expanding = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle(radius);
    glutSwapBuffers();
}

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Pulsing Circle to Point");

    initOpenGL();
    glutDisplayFunc(display);
    glutTimerFunc(30, update, 0);

    glutMainLoop();
    return 0;
}
