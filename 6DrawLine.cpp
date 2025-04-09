#include <GL/freeglut.h>
#include <vector>
#include <iostream>

// Structure to store line information
struct Line {
    float startX, startY; // Start point of the line
    float endX, endY;     // End point of the line
    float thickness;      // Thickness of the line
};

std::vector<Line> lines; // List of all drawn lines
bool isDrawing = false;  // Flag to check if we are currently drawing a line
float currentThickness = 1.0f; // Current line thickness
float startX, startY;    // Start point of the current line
float endX, endY;        // End point of the current line

// Mouse click callback function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Start drawing: Set the start point
            isDrawing = true;
            startX = (float)x;
            startY = (float)y;
            endX = (float)x;
            endY = (float)y;
        }
        else if (state == GLUT_UP) {
            // Stop drawing: Save the line
            isDrawing = false;
            lines.push_back({ startX, startY, endX, endY, currentThickness });
            glutPostRedisplay(); // Request to redraw the screen
        }
    }
}

// Mouse motion callback function
void motion(int x, int y) {
    if (isDrawing) {
        // Update the end point of the line as the mouse moves
        endX = (float)x;
        endY = (float)y;
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

// Function to draw a line
void drawLine(float startX, float startY, float endX, float endY, float thickness) {
    glLineWidth(thickness); // Set the line thickness
    glBegin(GL_LINES);
    glVertex2f(startX, startY); // Start point
    glVertex2f(endX, endY);     // End point
    glEnd();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    // Draw all stored lines
    for (const auto& line : lines) {
        drawLine(line.startX, line.startY, line.endX, line.endY, line.thickness);
    }

    // Draw the current line being drawn (if any)
    if (isDrawing) {
        drawLine(startX, startY, endX, endY, currentThickness);
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
    glutCreateWindow("OpenGL Line Drawing with GLUT"); // Create a window

    init(); // Call initialization function

    // Register callback functions
    glutDisplayFunc(display); // Display callback
    glutMouseFunc(mouse);     // Mouse click callback
    glutMotionFunc(motion);   // Mouse motion callback
    glutMouseWheelFunc(mouseWheel); // Mouse wheel callback

    glutMainLoop(); // Enter the main event loop
    return 0;
}