#include <GL/glut.h>
#include <math.h>

float a = 1.0;
float b = 0.0;
float c = 0.0;
float theta = 0.0;

void drawParabola() {
	float x, y, x_rotation, y_rotation;
	float x_start = -5.0;
	float x_end = 5.0;
	float step = 0.1;

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);

	glBegin(GL_LINE_STRIP);
	for (x = x_start; x <= x_end; x += step) {

		y = a * (x * x) + b * x + c; //  y = ax^2 + bx + c

		x_rotation = x * cos(theta) - y * sin(theta);
		y_rotation = x * sin(theta) + y * cos(theta);

		glVertex2f(x_rotation, y_rotation);
	}
	glEnd();
}

void drawAxes() {
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);

	glBegin(GL_LINES);
	glVertex2f(-10.0f, 0.0f);
	glVertex2f(10.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0.0f, -10.0f);
	glVertex2f(0.0f, 10.0f);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	drawAxes();
	drawParabola();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		theta -= 0.1;
		glutPostRedisplay();
		break;
	case 'd':
		theta += 0.1;
		glutPostRedisplay();
		break;
	case 's':
		a = (a >= 3.0) ? 0.1 : a + 0.1;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Maleki - Parabolic");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}