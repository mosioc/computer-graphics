# OpenGL Graphics Projects

This repository contains my solutions to the Computer Graphics Course (Class 2025) at QIAU (Qazvin Islamic Azad University), demonstrating various graphics programming concepts and animations using OpenGL.

## Table of Contents
- [Projects Overview](#projects-overview)
  - [1. Screensaver](#1-screensaver)
  - [2. Functional Random Shape](#2-functional-random-shape)
  - [3. Animated Circle](#3-animated-circle)
  - [4. Advanced Animated Circles](#4-advanced-animated-circles)
  - [5. Clock](#5-clock)
  - [6. Drawing Implementations](#6-drawing-implementations)
  - [7. Mouse Drawing](#7-mouse-drawing)
  - [Bonus Project: Paint Design](#bonus-project-paint-design)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installing FreeGLUT and Configuring Visual Studio](#installing-freeglut-and-configuring-visual-studio)
  - [Building and Running](#building-and-running)
- [Controls](#controls)
- [Code Structure](#code-structure)
- [Contributing](#contributing)

## Projects Overview

### 1. Screensaver
Multiple implementations of screensavers demonstrating different visual effects:

**Basic Screensaver** (`1ScreenSaverBasic.cpp`):
- Simple moving lines with random colors
- Basic boundary collision detection
- Smooth animation

**Mystify Screensaver** (`1ScreenSaverMystify.cpp`):
- Complex pattern of moving lines
- Advanced color transitions
- Multiple line segments
- Mystifying visual effects

**Bubbles Screensaver** (`1ScreenSaverBubbles.cpp`):
- Animated bubble-like circles
- Floating movement patterns
- Color variations
- Bubble collision effects

**Ribbon Screensaver** (`1ScreenSaverRibbon.cpp`):
- Flowing ribbon-like patterns
- Smooth color gradients
- Wave-like movements
- Continuous flow animation

**3D Text Screensaver** (`1ScreenSaver3dText.cpp`):
- 3D text rendering
- Text rotation and movement
- Depth effects
- Dynamic lighting

**Key Features:**
- Random colored lines
- Continuous movement
- Boundary collision detection
- Smooth animation

**Code Example:**
```cpp
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

const int numPoints = 8; // number of points in the chain
struct Point {
    float x, y;  // position
    float dx, dy; // movement speed
    float r, g, b; // color
};

Point points[numPoints];

void initPoints() {
    srand(time(0));
    for (int i = 0; i < numPoints; i++) {
        points[i].x = ((rand() % 200) / 100.0f) - 1.0f;
        points[i].y = ((rand() % 200) / 100.0f) - 1.0f;
        points[i].dx = (rand() % 5 + 1) * 0.005f * (rand() % 2 ? 1 : -1);
        points[i].dy = (rand() % 5 + 1) * 0.005f * (rand() % 2 ? 1 : -1);
        points[i].r = (float)(rand() % 256) / 255.0f;
        points[i].g = (float)(rand() % 256) / 255.0f;
        points[i].b = (float)(rand() % 256) / 255.0f;
    }
}

void update(int value) {
    for (int i = 0; i < numPoints; i++) {
        points[i].x += points[i].dx;
        points[i].y += points[i].dy;
        if (points[i].x < -1.0f || points[i].x > 1.0f) points[i].dx *= -1;
        if (points[i].y < -1.0f || points[i].y > 1.0f) points[i].dy *= -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < numPoints; i++) {
        glColor3f(points[i].r, points[i].g, points[i].b);
        glVertex2f(points[i].x, points[i].y);
    }
    glVertex2f(points[0].x, points[0].y);
    glEnd();
    glutSwapBuffers();
}
```

### 2. Functional Random Shape
A program that draws random shapes based on keyboard input. Each key triggers a different shape-drawing function.

**Key Features:**
- 10 random points per shape
- Different shapes for different keys
- Color change functionality (C key)
- Random coordinate generation

**Code Example:**
```cpp
#include <GL/glut.h>
#include <vector>
#include <cstdlib>

struct Point {
    float x, y;
    float r, g, b;
};

std::vector<Point> points;
float currentColor[3] = {1.0f, 1.0f, 1.0f};

void generateRandomPoints() {
    points.clear();
    for (int i = 0; i < 10; i++) {
        Point p;
        p.x = ((rand() % 200) / 100.0f) - 1.0f;
        p.y = ((rand() % 200) / 100.0f) - 1.0f;
        p.r = currentColor[0];
        p.g = currentColor[1];
        p.b = currentColor[2];
        points.push_back(p);
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'c':
            currentColor[0] = (float)(rand() % 256) / 255.0f;
            currentColor[1] = (float)(rand() % 256) / 255.0f;
            currentColor[2] = (float)(rand() % 256) / 255.0f;
            break;
        case ' ': generateRandomPoints(); break;
    }
    glutPostRedisplay();
}
```

### 3. Animated Circle
Multiple implementations of circle animations:

**Basic Pulse** (`3CirclePulse.cpp`):
- Simple pulsating circle at the origin
- Smooth size transitions
- Continuous animation
- Basic radius control

**Pulse with Ghosting** (`3CirclePulseGhosting.cpp`):
- Pulsating circle with trailing effect
- Multiple circle instances
- Fade-out effect
- Layered animation

### 4. Advanced Animated Circles
Two advanced circle animations:

**Spiral Circle** (`4SprialCircle.cpp`):
- Spiral pattern movement
- Continuous rotation
- Dynamic size changes
- Smooth spiral growth

**Spiral Circle with Bouncing** (`4SpiralCircleBouncing.cpp`):
- Combined spiral and bouncing effects
- Physics-based bouncing
- Gravity simulation
- Complex movement patterns

### 5. Clock
A digital clock implementation with rotating hands and numbers.

**Key Features:**
- Hour, minute, and second hands
- Numbers aligned with hands
- Real-time updates
- Proper rotation angles

**Code Example:**
```cpp
#include <GL/glut.h>
#include <ctime>

void drawClockHand(float angle, float length, float width) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, length);
    glEnd();
    glPopMatrix();
}

void display() {
    time_t now = time(0);
    struct tm *ltm = localtime(&now);
    
    float hourAngle = (ltm->tm_hour % 12 + ltm->tm_min / 60.0f) * 30.0f;
    float minuteAngle = ltm->tm_min * 6.0f;
    float secondAngle = ltm->tm_sec * 6.0f;
    
    glClear(GL_COLOR_BUFFER_BIT);
    drawClockHand(hourAngle, 0.5f, 3.0f);
    drawClockHand(minuteAngle, 0.7f, 2.0f);
    drawClockHand(secondAngle, 0.8f, 1.0f);
    glutSwapBuffers();
}
```

### 6. Drawing Implementations
Multiple implementations of drawing different shapes and objects:

**Basic Line Drawing** (`6DrawLine.cpp`):
- Simple line drawing functionality
- Mouse interaction
- Basic line properties
- Interactive drawing

**Circle Drawing** (`6DrawLineCircle.cpp`):
- Circle drawing with mouse
- Radius control
- Smooth circle rendering
- Interactive circle creation

**Cube Drawing** (`6DrawLineCube.cpp`):
- 3D cube wireframe drawing
- Perspective projection
- Vertex manipulation
- Interactive cube creation

### 7. Mouse Drawing
Three variations of mouse-based drawing:

**Line Drawing:**
- Click and drag functionality
- Mouse wheel thickness control
- Real-time line preview

**Rectangle Drawing:**
- Click and drag for size
- Mouse wheel thickness control
- Real-time rectangle preview

**Circle Drawing:**
- Click and drag for radius
- Euclidean distance calculation
- Real-time circle preview

**Line Drawing Code Example:**
```cpp
#include <GL/glut.h>

struct Point {
    float x, y;
};

Point startPoint, currentPoint;
bool isDrawing = false;
float lineWidth = 1.0f;

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDrawing = true;
            startPoint.x = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
            startPoint.y = 1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f;
        } else if (state == GLUT_UP) {
            isDrawing = false;
        }
    }
}

void motion(int x, int y) {
    if (isDrawing) {
        currentPoint.x = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;
        currentPoint.y = 1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f;
        glutPostRedisplay();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (isDrawing) {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glVertex2f(startPoint.x, startPoint.y);
        glVertex2f(currentPoint.x, currentPoint.y);
        glEnd();
    }
    glutSwapBuffers();
}
```

## Bonus Project: Paint Design
A Paint-like application with various drawing tools and features.

**Features:**
- Multiple drawing tools
- Color selection
- Shape drawing
- Free-hand drawing
- Save/Load functionality

## Getting Started

### Prerequisites
- OpenGL
- GLUT (OpenGL Utility Toolkit)
- C++ compiler

### Installing FreeGLUT and Configuring Visual Studio

1. **Download FreeGLUT:**
   - Go to the FreeGLUT website: https://www.transmissionzero.co.uk/software/freeglut-devel/
   - Download the latest version of FreeGLUT for Windows (e.g., `freeglut-3.2.2.zip`)

2. **Extract FreeGLUT:**
   - Extract the downloaded zip file to a known location (e.g., `C:\Program Files\freeglut`)

3. **Configure Visual Studio:**
   - Open your Visual Studio project
   - Right-click on your project in Solution Explorer and select "Properties"
   - Under "Configuration Properties" > "VC++ Directories":
     - Add the FreeGLUT include directory to "Include Directories" (e.g., `C:\Program Files\freeglut\include`)
     - Add the FreeGLUT lib directory to "Library Directories" (e.g., `C:\Program Files\freeglut\lib\x64` for 64-bit)
   - Under "Configuration Properties" > "Linker" > "Input":
     - Add `freeglut.lib` to "Additional Dependencies"
   - Under "Configuration Properties" > "Build Events" > "Post-Build Event":
     - Add a command to copy `freeglut.dll` to your output directory:
       ```
       xcopy /y "C:\Program Files\freeglut\bin\x64\freeglut.dll" "$(OutDir)"
       ```

4. **Verify Installation:**
   - Create a simple test program:
     ```cpp
     #include <GL/glut.h>
     
     void display() {
         glClear(GL_COLOR_BUFFER_BIT);
         glBegin(GL_TRIANGLES);
         glVertex2f(-0.5f, -0.5f);
         glVertex2f(0.5f, -0.5f);
         glVertex2f(0.0f, 0.5f);
         glEnd();
         glutSwapBuffers();
     }
     
     int main(int argc, char** argv) {
         glutInit(&argc, argv);
         glutCreateWindow("FreeGLUT Test");
         glutDisplayFunc(display);
         glutMainLoop();
         return 0;
     }
     ```
   - If the program compiles and runs, showing a triangle, FreeGLUT is correctly configured

**Note:** Make sure to use the correct version (32-bit or 64-bit) of FreeGLUT libraries matching your project's configuration.

### Building and Running
1. Ensure you have the required dependencies installed
2. Compile the source files:
   ```bash
   g++ -o program source.cpp -lglut -lGL -lGLU
   ```
3. Run the executable:
   ```bash
   ./program
   ```

## Controls

### Screensaver
- No controls required
- Close window to exit

### Functional Random Shape
- Different keys for different shapes
- 'C' key to change colors

### Animated Circle
- No controls required
- Close window to exit

### Advanced Animated Circles
- No controls required
- Close window to exit

### Clock
- No controls required
- Close window to exit

### Mouse Drawing
- Left click and drag to draw
- Mouse wheel to change thickness
- Right click to clear

## Code Structure

Each project is contained in its own source file:
- `1ScreenSaver.cpp`
- `2FunctionalRandomShape.cpp`
- `3AnimatedCircle.cpp`
- `4AdvancedAnimatedCircles.cpp`
- `5Clock.cpp`
- `6MouseDrawing.cpp`
- `PaintDesign.cpp` (Bonus project)

## Contributing
Feel free to submit issues and enhancement requests.

