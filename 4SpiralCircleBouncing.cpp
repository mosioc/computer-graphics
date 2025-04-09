#include <GL/glut.h>
#include <cmath>

#define M_PI 3.14159265358979323846


float ballX = -1.0f;  // Initial X position of the ball
float ballY = 1.0f;   // Initial Y position of the ball (height from the ground)
float velocityY = 0.0f; // Vertical velocity of the ball
float velocityX = 0.005f; // Horizontal velocity (slower movement to stay in the window longer)
float gravity = -0.001f; // Gravitational acceleration (fall speed)
float bounceFactor = 0.8f; // How much velocity is retained after each bounce (less than 1.0 makes the ball lose energy)
float ballRadius = 0.1f; // Radius of the ball
float rotationAngle = 0.0f; // Rotation angle for the spinning ball
float rotationSpeed = 5.0f; // Initial rotation speed (degrees per frame)

// Function to draw the ball with a checkered pattern
void drawBall() {
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);  // Move the ball to its current position
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); // Rotate the ball around its center

    // Draw the ball with a checkered pattern
    const int numSlices = 12; // Number of slices (like pizza slices)
    const int numStacks = 6;  // Number of stacks (like layers)
    for (int i = 0; i < numSlices; i++) {
        float angle1 = i * 2.0f * M_PI / numSlices;
        float angle2 = (i + 1) * 2.0f * M_PI / numSlices;

        for (int j = 0; j < numStacks; j++) {
            float radius1 = ballRadius * j / numStacks;
            float radius2 = ballRadius * (j + 1) / numStacks;

            // Alternate colors for the checkered pattern
            if ((i + j) % 2 == 0) {
                glColor3f(1.0f, 1.0f, 1.0f); // White
            }
            else {
                glColor3f(0.0f, 0.0f, 0.0f); // Black
            }

            // Draw a quad for each segment
            glBegin(GL_QUADS);
            glVertex2f(radius1 * cos(angle1), radius1 * sin(angle1));
            glVertex2f(radius2 * cos(angle1), radius2 * sin(angle1));
            glVertex2f(radius2 * cos(angle2), radius2 * sin(angle2));
            glVertex2f(radius1 * cos(angle2), radius1 * sin(angle2));
            glEnd();
        }
    }

    glPopMatrix();
}

// Function to update the animation
void update(int value) {
    // Apply gravity to the vertical velocity
    velocityY += gravity;

    // Update the Y position of the ball
    ballY += velocityY;

    // Update the X position (horizontal movement)
    ballX += velocityX;

    // Check for collision with the ground (Y = -1.0f)
    if (ballY - ballRadius <= -1.0f) {
        ballY = -1.0f + ballRadius;  // Prevent the ball from going below the ground
        velocityY = -velocityY * bounceFactor;  // Reverse and reduce vertical velocity (bounce)
        if (fabs(velocityY) < 0.01f) {  // Stop bouncing when the ball's velocity is very small
            velocityY = 0.0f;
        }
    }

    // Check if the ball has gone off the right side of the window
    if (ballX - ballRadius > 1.0f) {
        ballX = -1.0f;  // Reset position to the left side
        ballY = 1.0f;   // Reset height to the top
        velocityY = 0.0f; // Reset vertical velocity
        velocityX = 0.005f; // Reset horizontal velocity (slower)
    }

    // Update rotation angle with decreasing speed
    rotationAngle -= rotationSpeed;  // Rotate the ball
    if (rotationAngle < 0.0f) {
        rotationAngle += 360.0f; // Keep the rotation angle within 0-360 degrees
    }

    // Gradually decrease rotation speed over time
    if (rotationSpeed > 0.0f) {
        rotationSpeed -= 0.01f; // Decrease rotation speed
        if (rotationSpeed < 0.0f) {
            rotationSpeed = 0.0f; // Stop rotation when speed reaches 0
        }
    }

    glutPostRedisplay();  // Redraw the screen
    glutTimerFunc(16, update, 0);  // Update every 16 ms (60 FPS)
}

// Display function to draw the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear the screen and depth buffer
    glLoadIdentity();  // Reset the modelview matrix

    // Move the camera away from the center
    glTranslatef(0.0f, 0.0f, -2.0f);

    // Draw the ball
    drawBall();

    glutSwapBuffers();  // Swap buffers to display the scene
}

// Initialization of OpenGL settings
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set the background color to black
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 2.0f / 1.0f, 0.1f, 100.0f);  // Set perspective projection with updated aspect ratio
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);  // Set the window size (wider width)
    glutCreateWindow("Bouncing Ball with Decreasing Rotation");  // Create the window

    initOpenGL();  // Initialize OpenGL settings

    glutDisplayFunc(display);  // Set the display function for rendering
    glutTimerFunc(16, update, 0);  // Set the update function for animation

    glutMainLoop();  // Enter the OpenGL main loop
    return 0;
}