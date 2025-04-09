#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include <cmath> // For sqrt and pow functions

#define M_PI 3.14159265358979323846

// Structure to store circle information
struct Circle {
    float centerX, centerY; // Center of the circle
    float radius;           // Radius of the circle
    float thickness;        // Thickness of the circle border
};

std::vector<Circle> circles; // List of all drawn circles
bool isDrawing = false;  // Flag to check if we are currently drawing a circle
float currentThickness = 1.0f; // Current border thickness
float centerX, centerY;  // Center of the current circle
float radius;            // Radius of the current circle

// Mouse click callback function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Start drawing: Set the center of the circle
            isDrawing = true;
            centerX = (float)x;
            centerY = (float)y;
            radius = 0.0f; // Initialize radius to 0
        }
        else if (state == GLUT_UP) {
            // Stop drawing: Save the circle
            isDrawing = false;
            circles.push_back({ centerX, centerY, radius, currentThickness });
            glutPostRedisplay(); // Request to redraw the screen
        }
    }
}

// Mouse motion callback function
void motion(int x, int y) {
    if (isDrawing) {
        // Calculate the radius using the Euclidean distance formula
        radius = sqrt(pow((float)x - centerX, 2) + pow((float)y - centerY, 2));
        glutPostRedisplay(); // Request to redraw the screen
    }
}

// Mouse wheel callback function
void mouseWheel(int wheel, int direction, int x, int y) {
    currentThickness += (float)direction; // Adjust thickness based on wheel direction
    if (currentThickness < 1.0f) currentThickness = 1.0f; // Minimum thickness
    if (currentThickness > 10.0f) currentThickness = 10.0f; // Maximum thickness
    glutPostRedisplay(); // Request to redraw the screen
}

// Function to draw a circle
void drawCircle(float centerX, float centerY, float radius, float thickness) {
    glLineWidth(thickness); // Set the border thickness
    glBegin(GL_LINE_LOOP);  // Draw a circle border
    const int numSegments = 100; // Number of segments to approximate the circle
    for (int i = 0; i < numSegments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    // Draw all stored circles
    for (const auto& circle : circles) {
        drawCircle(circle.centerX, circle.centerY, circle.radius, circle.thickness);
    }

    // Draw the current circle being drawn (if any)
    if (isDrawing) {
        drawCircle(centerX, centerY, radius, currentThickness);
    }

    glutSwapBuffers(); // Swap buffers to display the rendered frame
}

// Initialization function
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set up orthographic projection to match window coordinates
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(800, 600); // Set window size
    glutCreateWindow("OpenGL Circle Drawing with GLUT"); // Create a window

    init(); // Call initialization function

    // Register callback functions
    glutDisplayFunc(display); // Display callback
    glutMouseFunc(mouse);     // Mouse click callback
    glutMotionFunc(motion);   // Mouse motion callback
    glutMouseWheelFunc(mouseWheel); // Mouse wheel callback

    glutMainLoop(); // Enter the main event loop
    return 0;
}