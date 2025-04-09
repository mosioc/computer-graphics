#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

const int numLines = 10; // Number of lines

struct Line {
    float x1, y1, x2, y2;  // Start and end coordinates
    float dx1, dy1, dx2, dy2; // Movement direction and speed
    float r, g, b; // Line color
};

Line lines[numLines];

void initLines() {
    srand(time(0));
    for (int i = 0; i < numLines; i++) {
        lines[i].x1 = rand() % 800;
        lines[i].y1 = rand() % 600;
        lines[i].x2 = rand() % 800;
        lines[i].y2 = rand() % 600;

        lines[i].dx1 = (rand() % 5 + 1) * ((rand() % 2) ? 1 : -1);
        lines[i].dy1 = (rand() % 5 + 1) * ((rand() % 2) ? 1 : -1);
        lines[i].dx2 = (rand() % 5 + 1) * ((rand() % 2) ? 1 : -1);
        lines[i].dy2 = (rand() % 5 + 1) * ((rand() % 2) ? 1 : -1);

        lines[i].r = (float)(rand() % 256) / 255.0f;
        lines[i].g = (float)(rand() % 256) / 255.0f;
        lines[i].b = (float)(rand() % 256) / 255.0f;
    }
}

void update(int value) {
    for (int i = 0; i < numLines; i++) {
        lines[i].x1 += lines[i].dx1;
        lines[i].y1 += lines[i].dy1;
        lines[i].x2 += lines[i].dx2;
        lines[i].y2 += lines[i].dy2;

        if (lines[i].x1 < 0 || lines[i].x1 > 800) lines[i].dx1 *= -1;
        if (lines[i].y1 < 0 || lines[i].y1 > 600) lines[i].dy1 *= -1;
        if (lines[i].x2 < 0 || lines[i].x2 > 800) lines[i].dx2 *= -1;
        if (lines[i].y2 < 0 || lines[i].y2 > 600) lines[i].dy2 *= -1;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    for (int i = 0; i < numLines; i++) {
        glColor3f(lines[i].r, lines[i].g, lines[i].b);
        glVertex2f(lines[i].x1, lines[i].y1);
        glVertex2f(lines[i].x2, lines[i].y2);
    }
    glEnd();

    glutSwapBuffers();
}

void initOpenGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Random Moving Lines Screensaver");

    initOpenGL();
    initLines();

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}