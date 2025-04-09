#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

const int numPoints = 10;
struct Point {
    float x, y;
};

std::vector<Point> points;
float r = 1.0f, g = 1.0f, b = 1.0f;
int currentShape = 1;

void drawText(const std::string& text, float x, float y) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void generateRandomPoints() {
    points.clear();
    for (int i = 0; i < numPoints; i++) {
        points.push_back({ ((rand() % 200) / 100.0f) - 1.0f, ((rand() % 200) / 100.0f) - 1.0f });
    }
}

void drawPolygon() {
    glBegin(GL_LINE_LOOP);
    glColor3f(r, g, b);
    for (const auto& p : points) {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawCircle() {
    glBegin(GL_LINE_LOOP);
    glColor3f(r, g, b);
    int segments = 40;
    float radius = 0.5f;
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawSmoothCurve() {
    if (points.size() < 4) return;

    glColor3f(r, g, b);
    glBegin(GL_LINE_STRIP);

    for (size_t i = 1; i < points.size() - 2; i++) {
        for (float t = 0; t <= 1; t += 0.1f) {
            float t2 = t * t;
            float t3 = t2 * t;

            float p0x = points[i - 1].x, p1x = points[i].x, p2x = points[i + 1].x, p3x = points[i + 2].x;
            float p0y = points[i - 1].y, p1y = points[i].y, p2y = points[i + 1].y, p3y = points[i + 2].y;

            float x = 0.5f * ((-p0x + 3 * p1x - 3 * p2x + p3x) * t3 +
                (2 * p0x - 5 * p1x + 4 * p2x - p3x) * t2 +
                (-p0x + p2x) * t +
                2 * p1x);

            float y = 0.5f * ((-p0y + 3 * p1y - 3 * p2y + p3y) * t3 +
                (2 * p0y - 5 * p1y + 4 * p2y - p3y) * t2 +
                (-p0y + p2y) * t +
                2 * p1y);

            glVertex2f(x, y);
        }
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    drawText("Press 1: Random Polygon", -0.9f, 0.9f);
    drawText("Press 2: Circle", -0.9f, 0.85f);
    drawText("Press 3: Smooth Curve", -0.9f, 0.8f);
    drawText("Press C: Change Color", -0.9f, 0.75f);

    switch (currentShape) {
    case 1: drawPolygon(); break;
    case 2: drawCircle(); break;
    case 3: drawSmoothCurve(); break;
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1': currentShape = 1; generateRandomPoints(); break;
    case '2': currentShape = 2; break;
    case '3': currentShape = 3; generateRandomPoints(); break;
    case 'c': case 'C':
        r = (float)(rand() % 256) / 255.0f;
        g = (float)(rand() % 256) / 255.0f;
        b = (float)(rand() % 256) / 255.0f;
        break;
    }
    glutPostRedisplay();
}

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Interactive Random Shapes");

    initOpenGL();
    generateRandomPoints();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
