#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

const int numBubbles = 15;

struct Bubble {
    float x, y;
    float dx, dy;
    float radius;
    float r, g, b;
};

Bubble bubbles[numBubbles];
float aspectRatio = 1.0f;

void initBubbles() {
    srand(time(0));
    for (int i = 0; i < numBubbles; i++) {
        bubbles[i].x = ((rand() % 200) / 100.0f) - 1.0f;
        bubbles[i].y = ((rand() % 200) / 100.0f) - 1.0f;
        bubbles[i].dx = (rand() % 5 + 1) * 0.002f * (rand() % 2 ? 1 : -1);
        bubbles[i].dy = (rand() % 5 + 1) * 0.002f * (rand() % 2 ? 1 : -1);
        bubbles[i].radius = ((rand() % 20) + 5) / 100.0f;
        bubbles[i].r = (float)(rand() % 256) / 255.0f;
        bubbles[i].g = (float)(rand() % 256) / 255.0f;
        bubbles[i].b = (float)(rand() % 256) / 255.0f;
    }
}

void update(int value) {
    for (int i = 0; i < numBubbles; i++) {
        bubbles[i].x += bubbles[i].dx;
        bubbles[i].y += bubbles[i].dy;

        if (bubbles[i].x - bubbles[i].radius < -1.0f * aspectRatio || bubbles[i].x + bubbles[i].radius > 1.0f * aspectRatio)
            bubbles[i].dx *= -1;
        if (bubbles[i].y - bubbles[i].radius < -1.0f || bubbles[i].y + bubbles[i].radius > 1.0f)
            bubbles[i].dy *= -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawBubble(float x, float y, float radius, float r, float g, float b) {
    glColor4f(r, g, b, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * 3.14159f * i / 20;
        float dx = radius * cos(angle);
        float dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < numBubbles; i++) {
        drawBubble(bubbles[i].x, bubbles[i].y, bubbles[i].radius, bubbles[i].r, bubbles[i].g, bubbles[i].b);
    }
    glutSwapBuffers();
}

void resize(int width, int height) {
    if (height == 0) height = 1;
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1.0f)
        gluOrtho2D(-aspectRatio, aspectRatio, -1.0, 1.0);
    else
        gluOrtho2D(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio);
    glMatrixMode(GL_MODELVIEW);
}

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bubbles Screensaver");

    initOpenGL();
    initBubbles();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
