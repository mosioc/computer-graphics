#include <GL/glut.h>
#include <cstdlib>

struct Point {
    float pOfX, pOfY;
    float speedOfX, speedOfY;
} points[8];

float randomBetween(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}

void init() {
    for (int i = 0; i < 8; i++) {
        points[i].pOfX = randomBetween(-1.0f, 1.0f);
        points[i].pOfY = randomBetween(-1.0f, 1.0f);
        points[i].speedOfX = randomBetween(0.001f, 0.020f) * (rand() % 2 ? 1 : -1);  // pos + dir
        points[i].speedOfY = randomBetween(0.001f, 0.020f) * (rand() % 2 ? 1 : -1);
    }
}

void update(int) {
    for (int i = 0; i < 8; i++) {
        points[i].pOfX += points[i].speedOfX;
        points[i].pOfY += points[i].speedOfY;

        if (points[i].pOfX < -1.0f || points[i].pOfX > 1.0f) points[i].speedOfX *= -1;
        if (points[i].pOfY < -1.0f || points[i].pOfY > 1.0f) points[i].speedOfY *= -1;
    }
    glutPostRedisplay();
    glutTimerFunc(17, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 8; i++) {
        glVertex2f(points[i].pOfX, points[i].pOfY);
    }
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Maleki - Screensaver");
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(-1, 1, -1, 1);
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
}