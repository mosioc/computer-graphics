#include <GL/glut.h>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <stack>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define CIRCLE 4
#define TRIANGLE 5
#define BRUSH 6
#define SHAPE_TEXT 7
#define COLOR_PICKER 8
#define ERASER 9

#define BLACK 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define YELLOW 5
#define CYAN 6
#define MAGENTA 7
#define ORANGE 8
#define PURPLE 9
#define GRAY 10
#define BROWN 11

int width = 800, height = 800;
int currentShape = 0;
int currentColor = BLACK;
float currentThickness = 1.0f;
bool currentFilled = false;
int clickCount = 0;
float points[3][2];
bool showGrid = true;
bool isDrawingBrush = false;
bool isEditMode = false;
int selectedShapeIndex = -1;

bool isEnteringText = false;
string currentText = "";
float textX = 0, textY = 0;
bool showTextCursor = true;
int cursorTimer = 0;

float zoomFactor = 1.0f;
float panX = 0.0f, panY = 0.0f;
bool isPanning = false;
int lastPanX, lastPanY;
const float MIN_ZOOM = 0.1f;
const float MAX_ZOOM = 10.0f;
const float ZOOM_STEP = 0.1f;

float colors[12][3] = {
	{0.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 1.0f},
	{1.0f, 0.5f, 0.0f},
	{0.5f, 0.0f, 0.5f},
	{0.5f, 0.5f, 0.5f},
	{0.6f, 0.3f, 0.0f}
};

struct Shape {
	int type;
	int color;
	vector<pair<float, float>> points;
	int radius;
	bool filled;
	string text;
	float rotation;
	float scale;
	float thickness;

	Shape() : type(0), color(BLACK), radius(0), filled(false), rotation(0.0f), scale(1.0f), thickness(1.0f) {}
};

vector<Shape> shapes;
stack<vector<Shape>> undoStack;
stack<vector<Shape>> redoStack;

GLuint textureID = 0;
int imgWidth = 0, imgHeight = 0;

void screenToWorld(int screenX, int screenY, float& worldX, float& worldY) {
	worldX = (screenX - width * 0.1f - panX * zoomFactor) / zoomFactor;
	worldY = (screenY - panY * zoomFactor) / zoomFactor;
}

void setColor(int colorIndex) {
	glColor3f(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
}

void drawPoint(float x, float y, float rotation, float scale, float thickness) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-x, -y, 0.0f);
	glPointSize(thickness * 5.0f * zoomFactor);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
	glPopMatrix();
}

void drawLine(float x1, float y1, float x2, float y2, float rotation, float scale, float thickness) {
	float cx = (x1 + x2) / 2.0f;
	float cy = (y1 + y2) / 2.0f;
	glPushMatrix();
	glTranslatef(cx, cy, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-cx, -cy, 0.0f);
	glLineWidth(thickness * zoomFactor);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void drawRectangle(float x1, float y1, float x2, float y2, float rotation, float scale, bool filled, float thickness) {
	float cx = (x1 + x2) / 2.0f;
	float cy = (y1 + y2) / 2.0f;
	glPushMatrix();
	glTranslatef(cx, cy, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-cx, -cy, 0.0f);
	glLineWidth(thickness * zoomFactor);
	glPolygonMode(GL_FRONT_AND_BACK, filled ? GL_FILL : GL_LINE);
	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void drawCircle(float centerX, float centerY, float radius, float rotation, float scale, bool filled, float thickness) {
	glPushMatrix();
	glTranslatef(centerX, centerY, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-centerX, -centerY, 0.0f);
	glLineWidth(thickness * zoomFactor);
	glPolygonMode(GL_FRONT_AND_BACK, filled ? GL_FILL : GL_LINE);
	glBegin(filled ? GL_POLYGON : GL_LINE_LOOP);
	for (int i = 0; i < 36; ++i) {
		float angle = i * 10.0f * 3.14159f / 180.0f;
		float x = centerX + radius * cosf(angle);
		float y = centerY + radius * sinf(angle);
		glVertex2f(x, y);
	}
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float rotation, float scale, bool filled, float thickness) {
	float cx = (x1 + x2 + x3) / 3.0f;
	float cy = (y1 + y2 + y3) / 3.0f;
	glPushMatrix();
	glTranslatef(cx, cy, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-cx, -cy, 0.0f);
	glLineWidth(thickness * zoomFactor);
	glPolygonMode(GL_FRONT_AND_BACK, filled ? GL_FILL : GL_LINE);
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void drawBrush(const vector<pair<float, float>>& points, float rotation, float scale, float thickness) {
	if (points.empty()) return;
	float cx = 0.0f, cy = 0.0f;
	for (const auto& p : points) {
		cx += p.first;
		cy += p.second;
	}
	cx /= static_cast<float>(points.size());
	cy /= static_cast<float>(points.size());
	glPushMatrix();
	glTranslatef(cx, cy, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-cx, -cy, 0.0f);
	glPointSize(thickness * 5.0f * zoomFactor);
	glBegin(GL_POINTS);
	for (const auto& p : points) {
		glVertex2f(p.first, p.second);
	}
	glEnd();
	glPopMatrix();
}

void drawText(float x, float y, const string& text, float rotation, float scale) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 1.0f);
	glTranslatef(-x, -y, 0.0f);
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
	glPopMatrix();
}

void drawTextCursor(float x, float y) {
	if (showTextCursor) {
		glBegin(GL_LINES);
		glVertex2f(x, y - 5.0f);
		glVertex2f(x, y + 15.0f);
		glEnd();
	}
}

void drawBackground() {
	if (textureID == 0) return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPushMatrix();
	glTranslatef(width * 0.1f + panX * zoomFactor, panY * zoomFactor, 0.0f);
	glScalef(zoomFactor, zoomFactor, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(static_cast<float>(imgWidth), 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(static_cast<float>(imgWidth), static_cast<float>(imgHeight));
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, static_cast<float>(imgHeight));
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawHighlight(const Shape& shape) {
	glColor3f(1.0f, 1.0f, 0.0f);
	glLineWidth(2.0f * zoomFactor);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
	float cx = 0.0f, cy = 0.0f;
	size_t pointCount = shape.points.size();

	if (pointCount > 0) {
		for (const auto& p : shape.points) {
			cx += p.first;
			cy += p.second;
		}
		cx /= static_cast<float>(pointCount);
		cy /= static_cast<float>(pointCount);
	}

	if (shape.type == CIRCLE) {
		cx = shape.points[0].first;
		cy = shape.points[0].second;
		float r = shape.radius * shape.scale;
		float cosTheta = cosf(shape.rotation);
		float sinTheta = sinf(shape.rotation);
		vector<pair<float, float>> circlePoints = {
			{cx + r, cy}, {cx - r, cy}, {cx, cy + r}, {cx, cy - r}
		};
		for (const auto& p : circlePoints) {
			float x = p.first - cx;
			float y = p.second - cy;
			float rotX = cx + (x * cosTheta - y * sinTheta);
			float rotY = cy + (x * sinTheta + y * cosTheta);
			minX = min(minX, rotX);
			minY = min(minY, rotY);
			maxX = max(maxX, rotX);
			maxY = max(maxY, rotY);
		}
	}
	else if (shape.type == SHAPE_TEXT) {
		float textWidth = shape.text.length() * 10.0f * shape.scale;
		float textHeight = 20.0f * shape.scale;
		cx = shape.points[0].first;
		cy = shape.points[0].second;
		vector<pair<float, float>> textPoints = {
			{cx, cy}, {cx + textWidth, cy}, {cx + textWidth, cy - textHeight}, {cx, cy - textHeight}
		};
		float cosTheta = cosf(shape.rotation);
		float sinTheta = sinf(shape.rotation);
		for (const auto& p : textPoints) {
			float x = p.first - cx;
			float y = p.second - cy;
			float rotX = cx + (x * cosTheta - y * sinTheta);
			float rotY = cy + (x * sinTheta + y * cosTheta);
			minX = min(minX, rotX);
			minY = min(minY, rotY);
			maxX = max(maxX, rotX);
			maxY = max(maxY, rotY);
		}
	}
	else {
		float cosTheta = cosf(shape.rotation);
		float sinTheta = sinf(shape.rotation);
		for (const auto& p : shape.points) {
			float x = p.first - cx;
			float y = p.second - cy;
			float rotX = cx + (x * cosTheta - y * sinTheta) * shape.scale;
			float rotY = cy + (x * sinTheta + y * cosTheta) * shape.scale;
			minX = min(minX, rotX);
			minY = min(minY, rotY);
			maxX = max(maxX, rotX);
			maxY = max(maxY, rotY);
		}
	}

	glBegin(GL_LINE_LOOP);
	glVertex2f(minX - 5.0f, minY - 5.0f);
	glVertex2f(maxX + 5.0f, minY - 5.0f);
	glVertex2f(maxX + 5.0f, maxY + 5.0f);
	glVertex2f(minX - 5.0f, maxY + 5.0f);
	glEnd();
	glLineWidth(1.0f);
}

void drawShapes() {
	glPushMatrix();
	glTranslatef(width * 0.1f + panX * zoomFactor, panY * zoomFactor, 0.0f);
	glScalef(zoomFactor, zoomFactor, 1.0f);

	for (size_t i = 0; i < shapes.size(); ++i) {
		setColor(shapes[i].color);
		switch (shapes[i].type) {
		case POINT:
			drawPoint(shapes[i].points[0].first, shapes[i].points[0].second,
				shapes[i].rotation, shapes[i].scale, shapes[i].thickness);
			break;
		case LINE:
			drawLine(shapes[i].points[0].first, shapes[i].points[0].second,
				shapes[i].points[1].first, shapes[i].points[1].second,
				shapes[i].rotation, shapes[i].scale, shapes[i].thickness);
			break;
		case RECTANGLE:
			drawRectangle(shapes[i].points[0].first, shapes[i].points[0].second,
				shapes[i].points[1].first, shapes[i].points[1].second,
				shapes[i].rotation, shapes[i].scale, shapes[i].filled, shapes[i].thickness);
			break;
		case CIRCLE:
			drawCircle(shapes[i].points[0].first, shapes[i].points[0].second,
				static_cast<float>(shapes[i].radius), shapes[i].rotation, shapes[i].scale,
				shapes[i].filled, shapes[i].thickness);
			break;
		case TRIANGLE:
			drawTriangle(shapes[i].points[0].first, shapes[i].points[0].second,
				shapes[i].points[1].first, shapes[i].points[1].second,
				shapes[i].points[2].first, shapes[i].points[2].second,
				shapes[i].rotation, shapes[i].scale, shapes[i].filled, shapes[i].thickness);
			break;
		case BRUSH:
			drawBrush(shapes[i].points, shapes[i].rotation, shapes[i].scale, shapes[i].thickness);
			break;
		case SHAPE_TEXT:
			if (!shapes[i].text.empty()) {
				drawText(shapes[i].points[0].first, shapes[i].points[0].second,
					shapes[i].text, shapes[i].rotation, shapes[i].scale);
			}
			break;
		}
		if (static_cast<int>(i) == selectedShapeIndex && isEditMode) {
			drawHighlight(shapes[i]);
		}
	}

	glPopMatrix();
}

void drawSidebar() {
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0f, 0.0f, width * 0.1f, static_cast<float>(height));

	// Shape buttons
	for (int i = 1; i <= 9; ++i) {
		if (i == currentShape && !isEditMode) glColor3f(1.0f, 1.0f, 1.0f);
		else glColor3f(0.7f, 0.7f, 0.7f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		float buttonHeight = height * 0.07f;
		float buttonTop = height - i * buttonHeight;
		float buttonBottom = height - (i - 0.9f) * buttonHeight;

		glRectf(width * 0.01f, buttonTop, width * 0.09f, buttonBottom);
		glColor3f(0.0f, 0.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(width * 0.01f, buttonTop, width * 0.09f, buttonBottom);

		glColor3f(0.0f, 0.0f, 0.0f);
		float iconY = height - (i - 0.5f) * buttonHeight;
		float iconX = width * 0.05f;

		glPointSize(5.0f);
		glLineWidth(1.0f);
		switch (i) {
		case POINT:
			drawPoint(iconX, iconY, 0.0f, 1.0f, 1.0f);
			break;
		case LINE:
			drawLine(iconX - 10.0f, iconY - 5.0f, iconX + 10.0f, iconY + 5.0f, 0.0f, 1.0f, 1.0f);
			break;
		case RECTANGLE:
			drawRectangle(iconX - 10.0f, iconY - 5.0f, iconX + 10.0f, iconY + 5.0f, 0.0f, 1.0f, false, 1.0f);
			break;
		case CIRCLE:
			drawCircle(iconX, iconY, 10.0f, 0.0f, 1.0f, false, 1.0f);
			break;
		case TRIANGLE:
			drawTriangle(iconX, iconY + 8.0f, iconX - 8.0f, iconY - 4.0f, iconX + 8.0f, iconY - 4.0f, 0.0f, 1.0f, false, 1.0f);
			break;
		case BRUSH:
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			glVertex2f(iconX - 5.0f, iconY + 5.0f);
			glVertex2f(iconX, iconY);
			glVertex2f(iconX + 5.0f, iconY - 5.0f);
			glEnd();
			break;
		case SHAPE_TEXT:
			glLineWidth(2.0f);
			glBegin(GL_LINES);
			glVertex2f(iconX - 8.0f, iconY + 8.0f);
			glVertex2f(iconX + 8.0f, iconY + 8.0f);
			glVertex2f(iconX, iconY + 8.0f);
			glVertex2f(iconX, iconY - 8.0f);
			glEnd();
			glLineWidth(1.0f);
			break;
		case COLOR_PICKER:
			glBegin(GL_LINES);
			glVertex2f(iconX - 8.0f, iconY + 8.0f);
			glVertex2f(iconX + 8.0f, iconY - 8.0f);
			glVertex2f(iconX + 8.0f, iconY - 8.0f);
			glVertex2f(iconX, iconY);
			glEnd();
			break;
		case ERASER:
			glBegin(GL_QUADS);
			glVertex2f(iconX - 5.0f, iconY + 5.0f);
			glVertex2f(iconX + 5.0f, iconY + 5.0f);
			glVertex2f(iconX + 5.0f, iconY - 5.0f);
			glVertex2f(iconX - 5.0f, iconY - 5.0f);
			glEnd();
			break;
		}
	}

	// Edit button
	float editButtonTop = height - 10 * height * 0.07f;
	float editButtonBottom = height - 10.9f * height * 0.07f;
	glColor3f(isEditMode ? 1.0f : 0.7f, isEditMode ? 1.0f : 0.7f, isEditMode ? 1.0f : 0.7f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(width * 0.01f, editButtonTop, width * 0.09f, editButtonBottom);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(width * 0.01f, editButtonTop, width * 0.09f, editButtonBottom);
	glRasterPos2f(width * 0.03f, height - 10.4f * height * 0.07f);
	string editLabel = "Edit";
	for (char c : editLabel) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
	}

	// Fill button
	float fillButtonTop = height - 11 * height * 0.07f;
	float fillButtonBottom = height - 11.9f * height * 0.07f;
	glColor3f(currentFilled ? 1.0f : 0.7f, currentFilled ? 1.0f : 0.7f, currentFilled ? 1.0f : 0.7f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(width * 0.01f, fillButtonTop, width * 0.09f, fillButtonBottom);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(width * 0.01f, fillButtonTop, width * 0.09f, fillButtonBottom);
	glRasterPos2f(width * 0.03f, height - 11.4f * height * 0.07f);
	string fillLabel = "Fill";
	for (char c : fillLabel) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
	}


	float thicknessStartY = height - 12 * height * 0.07f;
	for (int i = 0; i < 3; ++i) {
		float thickness = (i == 0) ? 1.0f : (i == 1) ? 2.0f : 4.0f;
		glColor3f(currentThickness == thickness ? 1.0f : 0.7f, currentThickness == thickness ? 1.0f : 0.7f, currentThickness == thickness ? 1.0f : 0.7f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		float top = thicknessStartY - i * height * 0.05f;
		float bottom = thicknessStartY - (i + 0.9f) * height * 0.05f;
		glRectf(width * 0.01f, top, width * 0.09f, bottom);
		glColor3f(0.0f, 0.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glRectf(width * 0.01f, top, width * 0.09f, bottom);
		glLineWidth(thickness);
		glBegin(GL_LINES);
		glVertex2f(width * 0.02f, top - height * 0.025f);
		glVertex2f(width * 0.08f, top - height * 0.025f);
		glEnd();
		glLineWidth(1.0f);
	}

	float colorStartY = height - 15 * height * 0.07f;
	for (int i = 0; i < 12; ++i) {
		glColor3f(colors[i][0], colors[i][1], colors[i][2]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		float colorHeight = height * 0.05f;
		float top = colorStartY - (i / 2) * colorHeight;
		float bottom = colorStartY - ((i / 2) + 0.9f) * colorHeight;
		float left = width * (0.01f + (i % 2) * 0.04f);
		float right = width * (0.05f + (i % 2) * 0.04f);
		glRectf(left, bottom, right, top);

		if (i == currentColor) {
			glColor3f(1.0f, 1.0f, 1.0f);
			glLineWidth(2.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glRectf(left - 2.0f, bottom - 2.0f, right + 2.0f, top + 2.0f);
			glLineWidth(1.0f);
		}
	}
}

void drawGrid() {
	glPushMatrix();
	glTranslatef(width * 0.1f + panX * zoomFactor, panY * zoomFactor, 0.0f);
	glScalef(zoomFactor, zoomFactor, 1.0f);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x5555);
	glColor3f(0.75f, 0.75f, 0.75f);
	glLineWidth(1.0f);

	int gridSpacing = max(10, min(100, static_cast<int>(50.0f / zoomFactor)));

	glBegin(GL_LINES);
	for (int i = -5000; i <= 5000; i += gridSpacing) {
		glVertex2f(static_cast<GLfloat>(i), -5000.0f);
		glVertex2f(static_cast<GLfloat>(i), 5000.0f);
	}
	for (int i = -5000; i <= 5000; i += gridSpacing) {
		glVertex2f(-5000.0f, static_cast<GLfloat>(i));
		glVertex2f(5000.0f, static_cast<GLfloat>(i));
	}
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
}

void drawPreview() {
	glPushMatrix();
	glTranslatef(width * 0.1f + panX * zoomFactor, panY * zoomFactor, 0.0f);
	glScalef(zoomFactor, zoomFactor, 1.0f);

	setColor(currentColor);
	switch (currentShape) {
	case POINT: break;
	case LINE:
		if (clickCount == 1) {
			drawPoint(points[0][0], points[0][1], 0.0f, 1.0f, currentThickness);
			drawLine(points[0][0], points[0][1], points[1][0], points[1][1], 0.0f, 1.0f, currentThickness);
		}
		break;
	case RECTANGLE:
		if (clickCount == 1) {
			drawPoint(points[0][0], points[0][1], 0.0f, 1.0f, currentThickness);
			drawRectangle(points[0][0], points[0][1], points[1][0], points[1][1], 0.0f, 1.0f, currentFilled, currentThickness);
		}
		break;
	case CIRCLE:
		if (clickCount == 1) {
			drawPoint(points[0][0], points[0][1], 0.0f, 1.0f, currentThickness);
			float radius = sqrtf(powf(points[1][0] - points[0][0], 2.0f) +
				powf(points[1][1] - points[0][1], 2.0f));
			drawCircle(points[0][0], points[0][1], radius, 0.0f, 1.0f, currentFilled, currentThickness);
		}
		break;
	case TRIANGLE:
		if (clickCount >= 1) {
			drawPoint(points[0][0], points[0][1], 0.0f, 1.0f, currentThickness);
		}
		if (clickCount >= 2) {
			drawPoint(points[1][0], points[1][1], 0.0f, 1.0f, currentThickness);
			drawLine(points[0][0], points[0][1], points[1][0], points[1][1], 0.0f, 1.0f, currentThickness);
			drawLine(points[1][0], points[1][1], points[2][0], points[2][1], 0.0f, 1.0f, currentThickness);
			drawLine(points[2][0], points[2][1], points[0][0], points[0][1], 0.0f, 1.0f, currentThickness);
		}
		break;
	case BRUSH:
		if (isDrawingBrush && !shapes.empty()) {
			drawBrush(shapes.back().points, 0.0f, 1.0f, currentThickness);
		}
		break;
	case SHAPE_TEXT:
		if (isEnteringText) {
			if (!currentText.empty()) {
				drawText(textX, textY, currentText, 0.0f, 1.0f);
			}
			float cursorX = textX;
			if (!currentText.empty()) {
				cursorX += currentText.length() * 10.0f;
			}
			drawTextCursor(cursorX, textY);
		}
		break;
	}

	glPopMatrix();
}

void saveStateForUndo() {
	undoStack.push(shapes);
	while (!redoStack.empty()) redoStack.pop();
}

void undo() {
	if (!undoStack.empty()) {
		redoStack.push(shapes);
		shapes = undoStack.top();
		undoStack.pop();
		clickCount = 0;
		isEnteringText = false;
		currentText.clear();
		selectedShapeIndex = -1;
		glutPostRedisplay();
	}
}

void redo() {
	if (!redoStack.empty()) {
		undoStack.push(shapes);
		shapes = redoStack.top();
		redoStack.pop();
		clickCount = 0;
		isEnteringText = false;
		currentText.clear();
		selectedShapeIndex = -1;
		glutPostRedisplay();
	}
}

void zoomIn() {
	if (zoomFactor < MAX_ZOOM) {
		zoomFactor += ZOOM_STEP;
		glutPostRedisplay();
	}
}

void zoomOut() {
	if (zoomFactor > MIN_ZOOM) {
		zoomFactor -= ZOOM_STEP;
		glutPostRedisplay();
	}
}

void resetZoom() {
	zoomFactor = 1.0f;
	panX = panY = 0.0f;
	glutPostRedisplay();
}

void finishTextEntry() {
	if (isEnteringText && !currentText.empty()) {
		saveStateForUndo();
		Shape shape;
		shape.type = SHAPE_TEXT;
		shape.color = currentColor;
		shape.points.push_back({ textX, textY });
		shape.text = currentText;
		shape.rotation = 0.0f;
		shape.scale = 1.0f;
		shape.thickness = currentThickness;
		shapes.push_back(shape);
	}
	isEnteringText = false;
	currentText.clear();
	glutPostRedisplay();
}

void saveToFile(const string& filename) {
	ofstream file(filename);
	if (!file.is_open()) {
		cout << "Error: Could not open file " << filename << " for saving." << endl;
		return;
	}

	for (const Shape& shape : shapes) {
		file << shape.type << "," << shape.color << ",";
		for (size_t i = 0; i < 3; ++i) {
			if (i < shape.points.size()) {
				file << shape.points[i].first << "," << shape.points[i].second;
			}
			else {
				file << "0,0";
			}
			file << ",";
		}
		file << shape.radius << "," << (shape.filled ? 1 : 0) << ",";
		file << shape.rotation << "," << shape.scale << "," << shape.thickness << ",";
		string safeText = shape.text;
		for (char& c : safeText) {
			if (c == ',') c = '_';
		}
		file << safeText << "\n";
	}

	file.close();
	cout << "Drawing saved to " << filename << endl;
}

void loadFromFile(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error: Could not open file " << filename << " for loading." << endl;
		return;
	}

	saveStateForUndo();
	shapes.clear();

	string line;
	while (getline(file, line)) {
		Shape shape;
		stringstream ss(line);
		string temp;

		getline(ss, temp, ',');
		shape.type = stoi(temp);

		getline(ss, temp, ',');
		shape.color = stoi(temp);

		for (int i = 0; i < 3; ++i) {
			float x, y;
			getline(ss, temp, ',');
			x = stof(temp);
			getline(ss, temp, ',');
			y = stof(temp);
			if (x != 0.0f || y != 0.0f) {
				shape.points.push_back({ x, y });
			}
		}

		getline(ss, temp, ',');
		shape.radius = stoi(temp);

		getline(ss, temp, ',');
		shape.filled = stoi(temp) != 0;

		getline(ss, temp, ',');
		shape.rotation = stof(temp);

		getline(ss, temp, ',');
		shape.scale = stof(temp);

		getline(ss, temp, ',');
		shape.thickness = stof(temp);

		getline(ss, shape.text);
		for (char& c : shape.text) {
			if (c == '_') c = ',';
		}

		if (shape.type >= POINT && shape.type <= ERASER) {
			shapes.push_back(shape);
		}
	}

	file.close();
	clickCount = 0;
	isEnteringText = false;
	currentText.clear();
	selectedShapeIndex = -1;
	glutPostRedisplay();
	cout << "Drawing loaded from " << filename << endl;
}

bool loadBMP(const string& filename, unsigned char*& data, int& w, int& h) {
	ifstream file(filename, ios::binary);
	if (!file.is_open()) {
		cout << "Error: Could not open BMP file " << filename << endl;
		return false;
	}

	char header[54];
	file.read(header, 54);
	if (header[0] != 'B' || header[1] != 'M') {
		cout << "Error: Not a valid BMP file" << endl;
		file.close();
		return false;
	}

	w = *(int*)&header[18];
	h = *(int*)&header[22];
	int dataOffset = *(int*)&header[10];
	int dataSize = *(int*)&header[34];

	data = new unsigned char[dataSize];
	file.seekg(dataOffset, ios::beg);
	file.read((char*)data, dataSize);
	file.close();

	// BMP stores BGR, convert to RGB
	for (int i = 0; i < dataSize; i += 3) {
		swap(data[i], data[i + 2]);
	}

	return true;
}

void importImage(const string& filename) {
	unsigned char* data = nullptr;
	if (loadBMP(filename, data, imgWidth, imgHeight)) {
		if (textureID != 0) {
			glDeleteTextures(1, &textureID);
		}
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		delete[] data;
		glutPostRedisplay();
	}
}

float distance(float x1, float y1, float x2, float y2) {
	return sqrtf(powf(x2 - x1, 2.0f) + powf(y2 - y1, 2.0f));
}

float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3) {
	return fabsf((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f);
}

int findNearestShape(float x, float y) {
	float minDist = FLT_MAX;
	int nearestIndex = -1;

	for (size_t i = 0; i < shapes.size(); ++i) {
		const Shape& shape = shapes[i];
		float dist = FLT_MAX;
		float cx = 0.0f, cy = 0.0f;
		size_t pointCount = shape.points.size();
		bool isInside = false;

		if (pointCount > 0) {
			for (const auto& p : shape.points) {
				cx += p.first;
				cy += p.second;
			}
			cx /= static_cast<float>(pointCount);
			cy /= static_cast<float>(pointCount);
		}

		float cosTheta = cosf(-shape.rotation);
		float sinTheta = sinf(-shape.rotation);
		float px = x - cx;
		float py = y - cy;
		float localX = cx + (px * cosTheta - py * sinTheta) / shape.scale;
		float localY = cy + (px * sinTheta + py * cosTheta) / shape.scale;

		if (shape.type == RECTANGLE && shape.points.size() >= 2) {
			float x1 = shape.points[0].first;
			float y1 = shape.points[0].second;
			float x2 = shape.points[1].first;
			float y2 = shape.points[1].second;
			float minX = min(x1, x2);
			float maxX = max(x1, x2);
			float minY = min(y1, y2);
			float maxY = max(y1, y2);
			if (localX >= minX && localX <= maxX && localY >= minY && localY <= maxY) {
				isInside = true;
				dist = 0.0f;
			}
			else {
				for (const auto& p : shape.points) {
					dist = min(dist, distance(x, y, p.first, p.second));
				}
			}
		}
		else if (shape.type == TRIANGLE && shape.points.size() >= 3) {
			float x1 = shape.points[0].first;
			float y1 = shape.points[0].second;
			float x2 = shape.points[1].first;
			float y2 = shape.points[1].second;
			float x3 = shape.points[2].first;
			float y3 = shape.points[2].second;
			float A = triangleArea(x1, y1, x2, y2, x3, y3);
			float A1 = triangleArea(localX, localY, x2, y2, x3, y3);
			float A2 = triangleArea(x1, y1, localX, localY, x3, y3);
			float A3 = triangleArea(x1, y1, x2, y2, localX, localY);
			if (fabsf(A - (A1 + A2 + A3)) < 0.01f) {
				isInside = true;
				dist = 0.0f;
			}
			else {
				for (const auto& p : shape.points) {
					dist = min(dist, distance(x, y, p.first, p.second));
				}
			}
		}
		else if (shape.type == CIRCLE && shape.points.size() >= 1) {
			cx = shape.points[0].first;
			cy = shape.points[0].second;
			dist = distance(localX, localY, cx, cy);
			if (dist < static_cast<float>(shape.radius)) {
				isInside = true;
				dist = 0.0f;
			}
		}
		else if (shape.type == SHAPE_TEXT && shape.points.size() >= 1) {
			float textWidth = shape.text.length() * 10.0f;
			float textHeight = 20.0f;
			cx = shape.points[0].first;
			cy = shape.points[0].second;
			if (localX >= cx - 5.0f && localX <= cx + textWidth + 5.0f &&
				localY >= cy - textHeight - 5.0f && localY <= cy + 5.0f) {
				isInside = true;
				dist = 0.0f;
			}
		}
		else {
			for (const auto& p : shape.points) {
				float px = p.first - cx;
				float py = p.second - cy;
				float rotX = cx + (px * cosf(shape.rotation) - py * sinf(shape.rotation)) * shape.scale;
				float rotY = cy + (px * sinf(shape.rotation) + py * cosf(shape.rotation)) * shape.scale;
				dist = min(dist, distance(x, y, rotX, rotY));
			}
		}

		if (isInside || dist < 20.0f) {
			if (dist < minDist) {
				minDist = dist;
				nearestIndex = static_cast<int>(i);
			}
		}
	}
	return nearestIndex;
}

void rotateShape(int index, float angle) {
	if (index < 0 || static_cast<size_t>(index) >= shapes.size()) return;
	saveStateForUndo();
	shapes[index].rotation += angle;
	glutPostRedisplay();
}

void resizeShape(int index, float factor) {
	if (index < 0 || static_cast<size_t>(index) >= shapes.size()) return;
	saveStateForUndo();
	shapes[index].scale *= factor;
	if (shapes[index].type == CIRCLE) {
		shapes[index].radius = static_cast<int>(shapes[index].radius * factor);
	}
	glutPostRedisplay();
}

void toggleFill(int index) {
	if (index < 0 || static_cast<size_t>(index) >= shapes.size()) return;
	saveStateForUndo();
	shapes[index].filled = !shapes[index].filled;
	glutPostRedisplay();
}

void setThickness(int index, float thickness) {
	if (index < 0 || static_cast<size_t>(index) >= shapes.size()) return;
	saveStateForUndo();
	shapes[index].thickness = thickness;
	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	drawSidebar();
	if (showGrid) drawGrid();
	drawShapes();
	if (!isEditMode) drawPreview();

	cursorTimer++;
	if (cursorTimer > 30) {
		showTextCursor = !showTextCursor;
		cursorTimer = 0;
		if (isEnteringText) glutPostRedisplay();
	}

	glutSwapBuffers();
}

bool handleSidebarClick(int x, int y) {
	if (x < width * 0.1f) {
		// Shape buttons
		for (int i = 1; i <= 9; ++i) {
			float buttonHeight = height * 0.07f;
			float top = height - (i - 0.9f) * buttonHeight;
			float bottom = height - i * buttonHeight;
			if (y >= bottom && y <= top) {
				if (isEnteringText) {
					finishTextEntry();
				}
				isEditMode = false;
				selectedShapeIndex = -1;
				currentShape = i;
				clickCount = 0;
				isDrawingBrush = (i == BRUSH);
				glutPostRedisplay();
				return true;
			}
		}

		// Edit button
		float editButtonTop = height - 10 * height * 0.07f;
		float editButtonBottom = height - 10.9f * height * 0.07f;
		if (y >= editButtonBottom && y <= editButtonTop) {
			if (isEnteringText) {
				finishTextEntry();
			}
			isEditMode = !isEditMode;
			currentShape = 0;
			clickCount = 0;
			isDrawingBrush = false;
			selectedShapeIndex = -1;
			glutPostRedisplay();
			return true;
		}


		float fillButtonTop = height - 11 * height * 0.07f;
		float fillButtonBottom = height - 11.9f * height * 0.07f;
		if (y >= fillButtonBottom && y <= fillButtonTop) {
			currentFilled = !currentFilled;
			glutPostRedisplay();
			return true;
		}


		float thicknessStartY = height - 12 * height * 0.07f;
		for (int i = 0; i < 3; ++i) {
			float top = thicknessStartY - i * height * 0.05f;
			float bottom = thicknessStartY - (i + 0.9f) * height * 0.05f;
			if (y >= bottom && y <= top) {
				currentThickness = (i == 0) ? 1.0f : (i == 1) ? 2.0f : 4.0f;
				glutPostRedisplay();
				return true;
			}
		}

		float colorStartY = height - 15 * height * 0.07f;
		for (int i = 0; i < 12; ++i) {
			float colorHeight = height * 0.05f;
			float top = colorStartY - (i / 2) * colorHeight;
			float bottom = colorStartY - ((i / 2) + 0.9f) * colorHeight;
			float left = width * (0.01f + (i % 2) * 0.04f);
			float right = width * (0.05f + (i % 2) * 0.04f);
			if (y >= bottom && y <= top && x >= left && x <= right) {
				currentColor = i;
				glutPostRedisplay();
				return true;
			}
		}
		return true;
	}
	return false;
}

void mouseClick(int button, int state, int x, int y) {
	y = height - y;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (handleSidebarClick(x, y)) {
			return;
		}

		if (x >= width * 0.1f) {
			float worldX, worldY;
			screenToWorld(x, y, worldX, worldY);

			if (isEditMode) {
				selectedShapeIndex = findNearestShape(worldX, worldY);
				glutPostRedisplay();
				return;
			}

			if (currentShape == COLOR_PICKER) {
				int index = findNearestShape(worldX, worldY);
				if (index != -1) {
					currentColor = shapes[index].color;
					glutPostRedisplay();
				}
				return;
			}

			if (currentShape == ERASER) {
				int index = findNearestShape(worldX, worldY);
				if (index != -1) {
					saveStateForUndo();
					if (shapes[index].type == BRUSH) {
						auto& points = shapes[index].points;
						points.erase(
							remove_if(points.begin(), points.end(),
								[&](const auto& p) {
									return distance(p.first, p.second, worldX, worldY) < 10.0f;
								}),
							points.end());
						if (points.empty()) {
							shapes.erase(shapes.begin() + index);
						}
					}
					else {
						shapes.erase(shapes.begin() + index);
					}
					selectedShapeIndex = -1;
					glutPostRedisplay();
				}
				return;
			}

			if (currentShape > 0) {
				if (currentShape == SHAPE_TEXT) {
					if (isEnteringText) {
						finishTextEntry();
					}
					isEnteringText = true;
					textX = worldX;
					textY = worldY;
					currentText.clear();
					showTextCursor = true;
					cursorTimer = 0;
				}
				else if (currentShape == BRUSH) {
					saveStateForUndo();
					isDrawingBrush = true;
					Shape shape;
					shape.type = BRUSH;
					shape.color = currentColor;
					shape.points.push_back({ worldX, worldY });
					shape.rotation = 0.0f;
					shape.scale = 1.0f;
					shape.thickness = currentThickness;
					shapes.push_back(shape);
				}
				else {
					points[clickCount][0] = worldX;
					points[clickCount][1] = worldY;
					if (clickCount < 2) {
						points[clickCount + 1][0] = worldX;
						points[clickCount + 1][1] = worldY;
					}
					clickCount++;
				}

				switch (currentShape) {
				case POINT:
				{
					saveStateForUndo();
					Shape shape;
					shape.type = POINT;
					shape.color = currentColor;
					shape.points.push_back({ worldX, worldY });
					shape.rotation = 0.0f;
					shape.scale = 1.0f;
					shape.thickness = currentThickness;
					shapes.push_back(shape);
					clickCount = 0;
				}
				break;
				case LINE:
					if (clickCount == 2) {
						saveStateForUndo();
						Shape shape;
						shape.type = LINE;
						shape.color = currentColor;
						shape.points.push_back({ points[0][0], points[0][1] });
						shape.points.push_back({ points[1][0], points[1][1] });
						shape.rotation = 0.0f;
						shape.scale = 1.0f;
						shape.thickness = currentThickness;
						shapes.push_back(shape);
						clickCount = 0;
					}
					break;
				case RECTANGLE:
					if (clickCount == 2) {
						saveStateForUndo();
						Shape shape;
						shape.type = RECTANGLE;
						shape.color = currentColor;
						shape.points.push_back({ points[0][0], points[0][1] });
						shape.points.push_back({ points[1][0], points[1][1] });
						shape.rotation = 0.0f;
						shape.scale = 1.0f;
						shape.filled = currentFilled;
						shape.thickness = currentThickness;
						shapes.push_back(shape);
						clickCount = 0;
					}
					break;
				case CIRCLE:
					if (clickCount == 2) {
						saveStateForUndo();
						Shape shape;
						shape.type = CIRCLE;
						shape.color = currentColor;
						shape.points.push_back({ points[0][0], points[0][1] });
						shape.radius = static_cast<int>(distance(points[0][0], points[0][1], points[1][0], points[1][1]));
						shape.rotation = 0.0f;
						shape.scale = 1.0f;
						shape.filled = currentFilled;
						shape.thickness = currentThickness;
						shapes.push_back(shape);
						clickCount = 0;
					}
					break;
				case TRIANGLE:
					if (clickCount == 3) {
						saveStateForUndo();
						Shape shape;
						shape.type = TRIANGLE;
						shape.color = currentColor;
						shape.points.push_back({ points[0][0], points[0][1] });
						shape.points.push_back({ points[1][0], points[1][1] });
						shape.points.push_back({ points[2][0], points[2][1] });
						shape.rotation = 0.0f;
						shape.scale = 1.0f;
						shape.filled = currentFilled;
						shape.thickness = currentThickness;
						shapes.push_back(shape);
						clickCount = 0;
					}
					break;
				}
				glutPostRedisplay();
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && currentShape == BRUSH) {
		isDrawingBrush = false;
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		if (x >= width * 0.1f) {
			isPanning = true;
			lastPanX = x;
			lastPanY = y;
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		isPanning = false;
	}
	else if (x >= width * 0.1f) {
		if (button == 3 && state == GLUT_DOWN) {
			zoomIn();
		}
		else if (button == 4 && state == GLUT_DOWN) {
			zoomOut();
		}
	}
}

void mouseMotion(int x, int y) {
	y = height - y;

	if (isPanning && x >= width * 0.1f) {
		panX += (x - lastPanX) / zoomFactor;
		panY += (y - lastPanY) / zoomFactor;
		lastPanX = x;
		lastPanY = y;
		glutPostRedisplay();
		return;
	}

	if (!isEditMode && x >= width * 0.1f) {
		float worldX, worldY;
		screenToWorld(x, y, worldX, worldY);

		if (isDrawingBrush && currentShape == BRUSH) {
			if (!shapes.empty() && shapes.back().type == BRUSH) {
				shapes.back().points.push_back({ worldX, worldY });
				glutPostRedisplay();
			}
		}
		else if (clickCount > 0) {
			points[clickCount][0] = worldX;
			points[clickCount][1] = worldY;
			glutPostRedisplay();
		}
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (isEnteringText) {
		if (key == 13 || key == 10) {
			finishTextEntry();
		}
		else if (key == 27) {
			isEnteringText = false;
			currentText.clear();
			glutPostRedisplay();
		}
		else if (key == 8) {
			if (!currentText.empty()) {
				currentText.pop_back();
				glutPostRedisplay();
			}
		}
		else if (key >= 32 && key <= 126) {
			currentText += key;
			glutPostRedisplay();
		}
		return;
	}

	switch (key) {
	case 'g': case 'G':
		showGrid = !showGrid;
		glutPostRedisplay();
		break;
	case 'c': case 'C':
		saveStateForUndo();
		shapes.clear();
		clickCount = 0;
		selectedShapeIndex = -1;
		glutPostRedisplay();
		break;
	case 'u': case 'U':
		undo();
		break;
	case 'r': case 'R':
		redo();
		break;
	case '+': case '=':
		zoomIn();
		break;
	case '-': case '_':
		zoomOut();
		break;
	case '0':
		resetZoom();
		break;
	case '1': case '2': case '3': case '4': case '5': case '6':
	case '7': case '8': case '9':
		if (key - '1' < 12) {
			currentColor = key - '1';
			glutPostRedisplay();
		}
		break;
	case 't': case 'T':
		if (isEnteringText) {
			finishTextEntry();
		}
		isEditMode = false;
		selectedShapeIndex = -1;
		currentShape = SHAPE_TEXT;
		clickCount = 0;
		isDrawingBrush = false;
		glutPostRedisplay();
		break;
	case 'e': case 'E':
		if (isEnteringText) {
			finishTextEntry();
		}
		isEditMode = !isEditMode;
		currentShape = 0;
		clickCount = 0;
		isDrawingBrush = false;
		selectedShapeIndex = -1;
		glutPostRedisplay();
		break;
	case 'p': case 'P':
		currentShape = COLOR_PICKER;
		isEditMode = false;
		clickCount = 0;
		isDrawingBrush = false;
		glutPostRedisplay();
		break;
	case 'x': case 'X':
		currentShape = ERASER;
		isEditMode = false;
		clickCount = 0;
		isDrawingBrush = false;
		glutPostRedisplay();
		break;
	case 'f': case 'F':
		currentFilled = !currentFilled;
		glutPostRedisplay();
		break;
	case 's': case 'S':
		saveToFile("drawing.txt");
		break;
	case 'l': case 'L':
		loadFromFile("drawing.txt");
		break;
	case 'i': case 'I':
		importImage("image.bmp");
		break;
	case 27:
		if (isEnteringText) {
			isEnteringText = false;
			currentText.clear();
			glutPostRedisplay();
		}
		else {
			exit(0);
		}
		break;
	}
}

void menu(int value) {
	if (isEnteringText && ((value >= 20 && value <= 28) || value == 9 || value == 14 || (value >= 30 && value <= 34))) {
		finishTextEntry();
	}

	switch (value) {
	case 1:
		saveStateForUndo();
		shapes.clear();
		clickCount = 0;
		isEnteringText = false;
		currentText.clear();
		selectedShapeIndex = -1;
		glutPostRedisplay();
		break;
	case 2:
		showGrid = !showGrid;
		glutPostRedisplay();
		break;
	case 3:
		exit(0);
		break;
	case 4:
		undo();
		break;
	case 5:
		redo();
		break;
	case 6:
		zoomIn();
		break;
	case 7:
		zoomOut();
		break;
	case 8:
		resetZoom();
		break;
	case 9:
		saveToFile("drawing.txt");
		break;
	case 14:
		loadFromFile("drawing.txt");
		break;
	case 15:
		importImage("image.bmp");
		break;
	case 16: case 17: case 18: case 19: case 20: case 21:
	case 22: case 23: case 24: case 25: case 26: case 27:
		currentColor = value - 16;
		glutPostRedisplay();
		break;
	case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35: case 36:
		isEditMode = false;
		selectedShapeIndex = -1;
		currentShape = value - 27;
		clickCount = 0;
		isDrawingBrush = (value == 32);
		glutPostRedisplay();
		break;
	case 37:
		rotateShape(selectedShapeIndex, 3.14159f / 2.0f);
		break;
	case 38:
		resizeShape(selectedShapeIndex, 1.2f);
		break;
	case 39:
		resizeShape(selectedShapeIndex, 0.8f);
		break;
	case 40:
		toggleFill(selectedShapeIndex);
		break;
	case 41:
		setThickness(selectedShapeIndex, 1.0f);
		break;
	case 42:
		setThickness(selectedShapeIndex, 2.0f);
		break;
	case 43:
		setThickness(selectedShapeIndex, 4.0f);
		break;
	}
}

void createMenu() {
	int colorMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Black (1)", 16);
	glutAddMenuEntry("Red (2)", 17);
	glutAddMenuEntry("Green (3)", 18);
	glutAddMenuEntry("Blue (4)", 19);
	glutAddMenuEntry("White (5)", 20);
	glutAddMenuEntry("Yellow (6)", 21);
	glutAddMenuEntry("Cyan (7)", 22);
	glutAddMenuEntry("Magenta (8)", 23);
	glutAddMenuEntry("Orange (9)", 24);
	glutAddMenuEntry("Purple", 25);
	glutAddMenuEntry("Gray" , 26);
	glutAddMenuEntry("Brown", 27);

	int shapeMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Point", 28);
	glutAddMenuEntry("Line", 29);
	glutAddMenuEntry("Rectangle", 30);
	glutAddMenuEntry("Circle", 31);
	glutAddMenuEntry("Triangle", 32);
	glutAddMenuEntry("Brush", 33);
	glutAddMenuEntry("Text", 34);
	glutAddMenuEntry("Color Picker", 35);
	glutAddMenuEntry("Eraser", 36);

	int zoomMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Zoom In (+)", 6);
	glutAddMenuEntry("Zoom Out (-)", 7);
	glutAddMenuEntry("Reset Zoom (0)", 8);

	int editMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Rotate 90�", 37);
	glutAddMenuEntry("Scale Up (1.2x)", 38);
	glutAddMenuEntry("Scale Down (0.8x)", 39);
	glutAddMenuEntry("Toggle Fill", 40);
	glutAddMenuEntry("Thin Line", 41);
	glutAddMenuEntry("Medium Line", 42);
	glutAddMenuEntry("Thick Line", 43);

	glutCreateMenu(menu);
	glutAddSubMenu("Shapes", shapeMenu);
	glutAddSubMenu("Colors", colorMenu);
	glutAddSubMenu("Zoom", zoomMenu);
	glutAddSubMenu("Edit", editMenu);
	glutAddMenuEntry("Clear (c)", 1);
	glutAddMenuEntry("Toggle Grid (g)", 2);
	glutAddMenuEntry("Undo", 4);
	glutAddMenuEntry("Redo", 5);
	glutAddMenuEntry("Save (s)", 9);
	glutAddMenuEntry("Load (l)", 14);
	glutAddMenuEntry("Import Image (i)", 15);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void reshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, static_cast<GLdouble>(w), 0.0, static_cast<GLdouble>(h));
	glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(33, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Paint Enhanced");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);
	createMenu();
	glutTimerFunc(33, timer, 0);
	glutMainLoop();
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
	}
	return 0;
}
