#include <GL/glut.h>
#include <cmath>

float radius = 0.05f;  // Initial radius
float angle = 0.0f;  // Starting angle
float step = 0.01f;  // Step size for the spiral's radius increase
bool reverse = false;  // Flag to reverse the spiral direction
float reverseAngle = 0.0f;  // Reverse angle for moving back to the center

// Function to draw a spiral of circles
void drawSpiralCircles() {
    glBegin(GL_LINE_LOOP);  // Begin the line loop for drawing

    // Loop to draw multiple circles along the spiral
    for (float t = 0; t < 20; t += 0.1f) {
        float r = radius + t * step;  // Increase the radius for each step to create a spiral effect
        angle += 0.1f;  // Increment the angle to make the spiral turn
        if (reverse) {
            r -= t * step;  // Decrease radius when reversing
            reverseAngle -= 0.1f;  // Adjust reverse angle to go back to the center
        }
        float x = r * cos(angle);  // Calculate x based on radius and angle
        float y = r * sin(angle);  // Calculate y based on radius and angle

        // Set a dynamic color based on the radius (you can change this to any color scheme you prefer)
        float red = 0.5f + 0.5f * cos(r * 10.0f);
        float green = 0.5f + 0.5f * sin(r * 5.0f);
        float blue = 0.5f + 0.5f * sin(r * 3.0f);
        glColor3f(red, green, blue);  // Set the color of the circle

        glVertex2f(x, y);  // Add this point to the line loop
    }

    glEnd();  // End the line loop
}

// Function to update the state of the animation
void update(int value) {
    // Gradually increase or decrease the radius depending on the direction
    if (!reverse) {
        radius += 0.005f;  // Slowly increase the radius for the next circle
        if (radius > 0.5f) {
            reverse = true;  // Once the radius reaches a certain size, start reversing
        }
    }
    else {
        radius -= 0.005f;  // Slowly decrease the radius for the reverse movement
        if (radius < 0.05f) {
            reverse = false;  // Once the radius is back to the initial size, start expanding again
        }
    }

    glutPostRedisplay();  // Request a redraw
    glutTimerFunc(30, update, 0);  // Redraw the screen every 30 milliseconds
}

// Display function to draw everything
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    drawSpiralCircles();  // Draw the spiral of circles

    glutSwapBuffers();  // Swap buffers for smooth animation
}

// Initialization of OpenGL settings
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set the background color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);  // Set 2D orthographic projection
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);  // Set the window size
    glutCreateWindow("Spiral of Circles (Reverse)");  // Create the window

    initOpenGL();  // Initialize OpenGL settings

    glutDisplayFunc(display);  // Set the display function for rendering
    glutTimerFunc(30, update, 0);  // Set the update function for animation

    glutMainLoop();  // Enter the OpenGL main loop
    return 0;
}
