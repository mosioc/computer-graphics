#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <string>

#define M_PI 3.14159265358979323846

// Hand angles
float hourAngle = 0.0f;
float minuteAngle = 0.0f;
float secondAngle = 0.0f;

// Draw text at specific position and angle
void drawText(const char* text, float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);  // Negative angle for clockwise rotation
    glScalef(0.0005f, 0.0005f, 1.0f);  // Text scale

    for (int i = 0; text[i] != '\0'; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }
    glPopMatrix();
}

// Draw clock numbers (1-12)
void drawClockNumbers() {
    char number[3];
    const float radius = 0.75f;  // Slightly inside the clock circle
    const float scale = 0.0007f; // Scale for the static numbers

    for (int i = 1; i <= 12; i++) {
        // Calculate position for each number
        float angle = static_cast<float>(-i * 30.0 + 90.0);  // 30 degrees per hour, starting from 12 o'clock
        float radian = angle * static_cast<float>(M_PI) / 180.0f;
        float x = cos(radian) * radius;
        float y = sin(radian) * radius;

        // Convert number to string
        sprintf_s(number, sizeof(number), "%d", i);

        // Draw the number
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(scale, scale, 1.0f);

        // Calculate width for centering
        float width = 0;
        for (const char* p = number; *p; p++) {
            width += glutStrokeWidth(GLUT_STROKE_ROMAN, *p);
        }

        // Center the text (stroke height is approximately 100.0f for GLUT_STROKE_ROMAN)
        glTranslatef(-width / 2.0f, -50.0f, 0.0f);  // 50.0f is half of approximate stroke height

        // Draw each digit
        for (const char* p = number; *p; p++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
        }
        glPopMatrix();
    }
}

// Draw clock hand with corresponding number
void drawHand(float length, float angle, float width, const char* number) {
    float radian = (-angle + 90.0f) * static_cast<float>(M_PI) / 180.0f;  // Negative angle for clockwise rotation
    float x = cos(radian) * length;
    float y = sin(radian) * length;

    // Draw hand
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glEnd();

    // Draw number on the hand
    float textX = cos(radian) * (length * 0.8f);
    float textY = sin(radian) * (length * 0.8f);
    drawText(number, textX, textY, angle);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw clock circle
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float radian = static_cast<float>(i * M_PI) / 180.0f;
        glVertex2f(cos(radian) * 0.9f, sin(radian) * 0.9f);
    }
    glEnd();

    // Draw clock numbers
    glColor3f(0.7f, 0.7f, 0.7f);  // Light gray for static numbers
    drawClockNumbers();

    // Get current time safely
    time_t now = time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    char hour[3], minute[3], second[3];
    sprintf_s(hour, sizeof(hour), "%d", timeinfo.tm_hour);
    sprintf_s(minute, sizeof(minute), "%d", timeinfo.tm_min);
    sprintf_s(second, sizeof(second), "%d", timeinfo.tm_sec);

    // Calculate hand angles (positive for clockwise rotation)
    hourAngle = static_cast<float>((timeinfo.tm_hour % 12 + timeinfo.tm_min / 60.0) * 30.0);
    minuteAngle = static_cast<float>(timeinfo.tm_min * 6.0);
    secondAngle = static_cast<float>(timeinfo.tm_sec * 6.0);

    // Draw hands and numbers
    glColor3f(1.0f, 0.0f, 0.0f);  // Red for hour hand
    drawHand(0.5f, hourAngle, 3.0f, hour);

    glColor3f(0.0f, 1.0f, 0.0f);  // Green for minute hand
    drawHand(0.7f, minuteAngle, 2.0f, minute);

    glColor3f(0.0f, 0.0f, 1.0f);  // Blue for second hand
    drawHand(0.8f, secondAngle, 1.0f, second);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = static_cast<float>(w) / static_cast<float>(h);
    if (w >= h) {
        glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);
    }
    else {
        glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0);  // Update every second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Analog Clock with Moving Numbers");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}