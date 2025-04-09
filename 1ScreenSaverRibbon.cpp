#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>

const int numRibbons = 5;
const int trailLength = 50;

struct Ribbon {
    std::vector<float> x, y;
    float dx, dy;
    float r, g, b;
};

Ribbon ribbons[numRibbons];
float aspectRatio = 1.0f;

void initRibbons() {
    srand(time(0));
    for (int i = 0; i < numRibbons; i++) {
        ribbons[i].x.assign(trailLength, ((rand() % 200) / 100.0f) - 1.0f);
        ribbons[i].y.assign(trailLength, ((rand() % 200) / 100.0f) - 1.0f);
        ribbons[i].dx = (rand() % 5 + 1) * 0.002f * (rand() % 2 ? 1 : -1);
        ribbons[i].dy = (rand() % 5 + 1) * 0.002f * (rand() % 2 ? 1 : -1);
        ribbons[i].r = (float)(rand() % 256) / 255.0f;
        ribbons[i].g = (float)(rand() % 256) / 255.0f;
        ribbons[i].b = (float)(rand() % 256) / 255.0f;
    }
}

void update(int value) {
    for (int i = 0; i < numRibbons; i++) {
        for (int j = trailLength - 1; j > 0; j--) {
            ribbons[i].x[j] = ribbons[i].x[j - 1];
            ribbons[i].y[j] = ribbons[i].y[j - 1];
        }
        ribbons[i].x[0] += ribbons[i].dx;
        ribbons[i].y[0] += ribbons[i].dy;
        
        if (ribbons[i].x[0] < -1.0f * aspectRatio || ribbons[i].x[0] > 1.0f * aspectRatio) 
            ribbons[i].dx *= -1;
        if (ribbons[i].y[0] < -1.0f || ribbons[i].y[0] > 1.0f) 
            ribbons[i].dy *= -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawRibbon(const Ribbon& ribbon) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < trailLength; i++) {
        float alpha = (float)i / trailLength;
        glColor4f(ribbon.r, ribbon.g, ribbon.b, alpha);
        glVertex2f(ribbon.x[i], ribbon.y[i]);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3.0f);
    for (int i = 0; i < numRibbons; i++) {
        drawRibbon(ribbons[i]);
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
    glEnable(GL_LINE_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Ribbon Screensaver");

    initOpenGL();
    initRibbons();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
