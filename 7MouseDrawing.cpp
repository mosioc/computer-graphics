#include <GL/glut.h>
#include <vector>

using namespace std;

struct Point {
	float x, y;
	Point(float px, float py) : x(px), y(py) {
	}
};

vector<Point> points;

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);  


	glColor3f(1.0f, 1.0f, 1.0f);  
	glPointSize(8.0f); 

	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++) {
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glBegin(GL_LINES);
	for (int i = 1; i < points.size(); i++) {
		glVertex2f(points[i - 1].x, points[i - 1].y);
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glutSwapBuffers(); 
}

void mouseControl(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		points.push_back(Point(x, glutGet(GLUT_WINDOW_HEIGHT) - y));
	}
	glutPostRedisplay();  
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}

void resize(int w, int h) {
	glViewport(0, 0, w, h);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1); 
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Maleki - Mouse Points");
	init();
	glutDisplayFunc(draw);
	glutMouseFunc(mouseControl);
	glutReshapeFunc(resize);
	glutMainLoop();
}