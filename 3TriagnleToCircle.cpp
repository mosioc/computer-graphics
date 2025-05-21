#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

const float PI = 3.14f;
int numVertices = 3;
const int minVertices = 3;
const int maxVertices = 100;

bool autoMode = false;
bool increasing = true;

void drawShape(float R = 0.5f, float centerX = 0.0f, float centerY = 0.0f) {
	float angleStep = 2 * PI / numVertices;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numVertices; ++i) {
		float angle = i * angleStep;
		glColor3f(
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX);
		float x = centerX + R * cos(angle);
		float y = centerY + R * sin(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawShape();
	glutSwapBuffers();
}

void idle(int x) {
	if (autoMode) {
		if (increasing) {
			if (numVertices < maxVertices)
				++numVertices;
			else
				increasing = false;
		}
		else {
			if (numVertices > minVertices)
				--numVertices;
			else
				increasing = true;
		}
		glutPostRedisplay();
		glutTimerFunc(50, idle, 0);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ' ': 
		if (numVertices < maxVertices) {
			++numVertices;
			glutPostRedisplay();
		}
		break;
	case 'b': 
		if (numVertices > minVertices) {
			--numVertices;
			glutPostRedisplay();
		}
		break;
	case 'a': 
		autoMode = !autoMode;
		if (autoMode) idle(0);
		break;
	case 27: 
		exit(0);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Maleki - Triangle to Circle)");
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}