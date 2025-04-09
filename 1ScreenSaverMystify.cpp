#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Number of polygons
#define NUM_POLYGONS 4

// Number of vertices per polygon
#define VERTICES_PER_POLYGON 4

// Animation speed
#define ANIM_SPEED 0.005

// Color change speed
#define COLOR_CHANGE_SPEED 0.01

// Vertex structure
typedef struct {
    float x, y;
    float dx, dy;
} Vertex;

// Polygon structure
typedef struct {
    Vertex vertices[VERTICES_PER_POLYGON];
    float r, g, b;
    float dr, dg, db;
} Polygon;

// Global array of polygons
Polygon polygons[NUM_POLYGONS];

// Function prototypes
void initVertex(Vertex* vertex);
void initPolygon(Polygon* polygon);
void init(void);
void updateVertex(Vertex* vertex);
void updatePolygon(Polygon* polygon);
void display(void);
void reshape(int width, int height);
void animate(void);
void timer(int value);
void keyboard(unsigned char key, int x, int y);

// Initialize a random vertex
void initVertex(Vertex* vertex) {
    vertex->x = (float)(rand() % WINDOW_WIDTH);
    vertex->y = (float)(rand() % WINDOW_HEIGHT);

    // Random velocity between -1 and 1
    vertex->dx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    vertex->dy = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

    // Normalize the velocity and scale it
    float length = sqrtf(vertex->dx * vertex->dx + vertex->dy * vertex->dy);
    vertex->dx = vertex->dx / length * 2.0f;
    vertex->dy = vertex->dy / length * 2.0f;
}

// Initialize a random polygon
void initPolygon(Polygon* polygon) {
    int i;
    for (i = 0; i < VERTICES_PER_POLYGON; i++) {
        initVertex(&polygon->vertices[i]);
    }

    // Random color
    polygon->r = (float)rand() / RAND_MAX;
    polygon->g = (float)rand() / RAND_MAX;
    polygon->b = (float)rand() / RAND_MAX;

    // Random color change direction
    polygon->dr = ((float)rand() / RAND_MAX) * 2.0f * COLOR_CHANGE_SPEED - COLOR_CHANGE_SPEED;
    polygon->dg = ((float)rand() / RAND_MAX) * 2.0f * COLOR_CHANGE_SPEED - COLOR_CHANGE_SPEED;
    polygon->db = ((float)rand() / RAND_MAX) * 2.0f * COLOR_CHANGE_SPEED - COLOR_CHANGE_SPEED;
}

// Initialize all polygons
void init(void) {
    int i;
    srand((unsigned int)time(NULL));

    for (i = 0; i < NUM_POLYGONS; i++) {
        initPolygon(&polygons[i]);
    }

    // Set up OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(2.0f);
}

// Update vertex position
void updateVertex(Vertex* vertex) {
    // Update position
    vertex->x += vertex->dx;
    vertex->y += vertex->dy;

    // Bounce off walls
    if (vertex->x < 0 || vertex->x > WINDOW_WIDTH) {
        vertex->dx = -vertex->dx;
        vertex->x = (vertex->x < 0) ? 0 : WINDOW_WIDTH;
    }

    if (vertex->y < 0 || vertex->y > WINDOW_HEIGHT) {
        vertex->dy = -vertex->dy;
        vertex->y = (vertex->y < 0) ? 0 : WINDOW_HEIGHT;
    }
}

// Update polygon colors and positions
void updatePolygon(Polygon* polygon) {
    int i;
    // Update all vertices
    for (i = 0; i < VERTICES_PER_POLYGON; i++) {
        updateVertex(&polygon->vertices[i]);
    }

    // Update color
    polygon->r += polygon->dr;
    polygon->g += polygon->dg;
    polygon->b += polygon->db;

    // If color goes out of range, reverse direction
    if (polygon->r < 0.0f || polygon->r > 1.0f) {
        polygon->dr = -polygon->dr;
        polygon->r = (polygon->r < 0.0f) ? 0.0f : 1.0f;
    }

    if (polygon->g < 0.0f || polygon->g > 1.0f) {
        polygon->dg = -polygon->dg;
        polygon->g = (polygon->g < 0.0f) ? 0.0f : 1.0f;
    }

    if (polygon->b < 0.0f || polygon->b > 1.0f) {
        polygon->db = -polygon->db;
        polygon->b = (polygon->b < 0.0f) ? 0.0f : 1.0f;
    }
}

// Display function
void display(void) {
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < NUM_POLYGONS; i++) {
        Polygon* poly = &polygons[i];

        // Set color with transparency
        glColor4f(poly->r, poly->g, poly->b, 0.7f);

        // Draw polygon
        glBegin(GL_LINE_LOOP);
        for (j = 0; j < VERTICES_PER_POLYGON; j++) {
            glVertex2f(poly->vertices[j].x, poly->vertices[j].y);
        }
        glEnd();
    }

    glutSwapBuffers();
}

// Reshape function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

// Animation function
void animate(void) {
    int i;
    for (i = 0; i < NUM_POLYGONS; i++) {
        updatePolygon(&polygons[i]);
    }

    glutPostRedisplay();
}

// Timer function
void timer(int value) {
    animate();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Keyboard function to handle ESC key
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mystify Screensaver");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}