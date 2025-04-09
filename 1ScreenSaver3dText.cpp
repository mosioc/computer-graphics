#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

const int numPoints = 8; // number of points in the chain
struct Point {
    float x, y;  // position
    float dx, dy; // movement speed
    float r, g, b; // color
};#include <GL/glut.h>
#include <string.h>
#include <math.h>

float angle = 0.0f;
const char* text = "Windows";

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void draw3DLetter(char letter, float xOffset) {
    float width = 0.2f;    // Width of each letter
    float height = 0.4f;   // Height of each letter
    float depth = 0.1f;    // Depth for 3D effect

    glPushMatrix();
    glTranslatef(xOffset, 0.0f, 0.0f);  // Position letter
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotate around its own left edge

    // Front face with bitmap text
    glColor3f(1.0f, 1.0f, 1.0f);  // White front
    glRasterPos3f(width / 2 - 0.05f, 0.0f, depth / 2 + 0.01f); // Center text on front
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);

    // Back face (solid quad)
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);  // Gray back
    glVertex3f(0.0f, -height / 2, -depth / 2);
    glVertex3f(width, -height / 2, -depth / 2);
    glVertex3f(width, height / 2, -depth / 2);
    glVertex3f(0.0f, height / 2, -depth / 2);
    glEnd();

    // Sides for 3D effect
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);
    // Left side
    glVertex3f(0.0f, -height / 2, -depth / 2);
    glVertex3f(0.0f, -height / 2, depth / 2);
    glVertex3f(0.0f, height / 2, depth / 2);
    glVertex3f(0.0f, height / 2, -depth / 2);
    // Right side
    glVertex3f(width, -height / 2, -depth / 2);
    glVertex3f(width, -height / 2, depth / 2);
    glVertex3f(width, height / 2, depth / 2);
    glVertex3f(width, height / 2, -depth / 2);
    glEnd();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera
    gluLookAt(0.0f, 0.0f, 5.0f,   // Camera position
        0.0f, 0.0f, 0.0f,   // Look at point
        0.0f, 1.0f, 0.0f);  // Up vector

    // Center the entire word
    float textWidth = strlen(text) * 0.2f;  // Total width (0.2 per letter)
    glTranslatef(-textWidth / 2, 0.0f, 0.0f);  // Move to center screen

    // Draw each letter with its own rotation
    for (int i = 0; i < strlen(text); i++) {
        draw3DLetter(text[i], i * 0.2f);  // 0.2f spacing between letters
    }

    glutSwapBuffers();
}

void timer(int value) {
    angle += 2.0f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Rotating 3D Text");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    init();
    glutMainLoop();

    return 0;
}

Point points[numPoints];

void initPoints() {
    srand(time(0));
    for (int i = 0; i < numPoints; i++) {

        points[i].x = ((rand() % 200) / 100.0f) - 1.0f; // random between -1 to 1
        points[i].y = ((rand() % 200) / 100.0f) - 1.0f;

        points[i].dx = (rand() % 5 + 1) * 0.005f * (rand() % 2 ? 1 : -1);
        points[i].dy = (rand() % 5 + 1) * 0.005f * (rand() % 2 ? 1 : -1);

        points[i].r = (float)(rand() % 256) / 255.0f; // random between 0 to 1 
        points[i].g = (float)(rand() % 256) / 255.0f;
        points[i].b = (float)(rand() % 256) / 255.0f;
    }
}

void update(int value) {
    for (int i = 0; i < numPoints; i++) {

        points[i].x += points[i].dx;
        points[i].y += points[i].dy;

        //  If a point has moved outside the boundaries (-1.0 to 1.0) => reverses its direction 
        if (points[i].x < -1.0f || points[i].x > 1.0f) points[i].dx *= -1;
        if (points[i].y < -1.0f || points[i].y > 1.0f) points[i].dy *= -1;
    }

    // redraw
    glutPostRedisplay();

    // schedules "update" function to be called again after 16ms => 60fps 
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // connecetd lines (start) 
    glBegin(GL_LINE_STRIP); // connected lines

    for (int i = 0; i < numPoints; i++) {
        glColor3f(points[i].r, points[i].g, points[i].b);
        glVertex2f(points[i].x, points[i].y);
    }
    glVertex2f(points[0].x, points[0].y); // Close the shape
    glEnd();
    // connected lines (end) 

    // swaps the front and back buffers to display the rendered frame
    glutSwapBuffers();
}

void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //  gl projection matrix: transformation matrix that defines how 3D (or 2D) coordinates ...
    //  ... are mapped onto the 2D screen. 
    glMatrixMode(GL_PROJECTION);

    // reset pMatrix
    glLoadIdentity();

    // mapping 
    gluOrtho2D(-1, 1, -1, 1);
}

void reshape(int width, int height) {
    // Set the viewport to cover the new window size
    glViewport(0, 0, width, height);

    // Reset the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Adjust the orthographic projection to maintain the aspect ratio
    float aspectRatio = (float)width / (float)height; // width/height
    if (aspectRatio > 1.0f) {
        // Window is wider than tall
        gluOrtho2D(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);
    }
    else {
        // Window is taller than wide
        gluOrtho2D(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio);
    }

    // Switch back to the modelview matrix
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {

    // defaults 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    
    // naming the window
    glutCreateWindow("Connected Moving Lines Screensaver");

    initOpenGL();
    initPoints();
    glutReshapeFunc(reshape); // register the reshape function

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
