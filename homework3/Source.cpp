#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <gl/glut.h>
#include <math.h>
#include <float.h>
#include <windows.h>
#include <assert.h>

#define DEFAULT_SIZE 300
#define PI 3.1415926535897

unsigned char prevKey;
int height = DEFAULT_SIZE;
int width = DEFAULT_SIZE;

class Point {
private:
	float x, y;
public:
	Point() {
		this->x = 0;
		this->y = 0;
	}
	void setY(float y) {
		this->y = y;
	}
	void setX(float x) {
		this->x = x;
	}
	float getY() {
		return this->y;
	}
	float getX() {
		return this->x;
	}
};

class Grill {
private:
	float x_ratio, y_ratio;
	int size;
	Point top, bottom;
public:
	Grill(int size){
		bottom.setX(-0.8);
		bottom.setY(-0.8);
		top.setX(0.8);
		top.setY(0.8);
		this->size = size;
		refreshRatio();
	}

	void refreshRatio() {
		fprintf(stdout, "%f	%f\n", this->x_ratio, this->y_ratio);
		this->x_ratio = (fabs(bottom.getX()) + fabs(top.getX())) / (size - 1);
		this->y_ratio = (fabs(bottom.getY()) + fabs(top.getY())) / (size - 1);
		fprintf(stdout, "%f	%f\n---------------------------------\n", this->x_ratio, this->y_ratio);

	}

	void resize(int height, int width) {
		{
			float result = 0;
			if (height > width)
			{
				result = height - width;
				result = result / 2;
				result = height / result;
				bottom.setX(-0.8);
				bottom.setY(-0.8 + 1.6 / result);
				top.setX(0.8);
				top.setY(0.8 - 1.6 / result);
				refreshRatio();
			}
			else {
				result = width - height;
				result = result / 2;
				if (height < width)
				{
					result = width / result;
					bottom.setX(-0.8 + 1.6 / result);
					bottom.setY(-0.8);
					top.setX(0.8 - 1.6 / result);
					top.setY(0.8);
					refreshRatio();
				}
				else {
					bottom.setX(-0.8);
					bottom.setY(-0.8);
					top.setX(0.8);
					top.setY(0.8);
					refreshRatio();

				}
			}
		}
	}

	void draw() {
		glColor3f(0.1, 0.1, 0.1);
		glLineWidth(1);
		glBegin(GL_LINES);
	//	fprintf(stdout, "%f	%f\n",x_ratio, y_ratio);
		for (float x = bottom.getX(); x <= top.getX(); x += x_ratio) {
			glVertex2f(x, bottom.getY());
			glVertex2f(x, top.getY());
		}
		glVertex2f(top.getX(), bottom.getY());
		glVertex2f(top.getX(), top.getY());
		glEnd();
		glBegin(GL_LINES);
		for (float y = bottom.getY(); y <= top.getY(); y += y_ratio) {
			glVertex2f(bottom.getX(), y);
			glVertex2f(top.getX(), y);
		}
		glVertex2f(bottom.getX(), top.getY());
		glVertex2f(top.getX(), top.getY());
		glEnd();
	}

	void writePixel(float x, float y, int nivel) {
		glPointSize(5);
		x = x * x_ratio + bottom.getX();
		y = y * y_ratio + bottom.getY();
		float raza = 0;
		glColor3f(0,0,1);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
/*		if (ratiax <= ratiay)
			raza = ratiax * 0.4;
		else
			raza = ratiay * 0.4;

		drawFilledCircle(x, y, raza);
		for (int i = 1; i <= nivel; i++) {

			drawFilledCircle(x - i * ratiax, y, raza);
			drawFilledCircle(x + i * ratiax, y, raza);
			drawFilledCircle(x, y + i * ratiay, raza);
			drawFilledCircle(x, y - i * ratiay, raza);
		}*/
	}

	void plotLineLow(float x0, float y0, float x1, float y1, int nivel) {
		float dx = x1 - x0;
		float dy = y1 - y0;
		float yi = 1;
		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}
		float D = 2 * dy - dx;
		float y = y0;

		for (float x = x0; x <= x1; x += 1) {
			writePixel(x, y, nivel);
			if (D > 0) {
				y = y + yi;
				D = D - 2 * dx;
			}
			D = D + 2 * dy;
		}
	}

	void plotLineHigh(float x0, float y0, float x1, float y1, int nivel) {
		float dx = x1 - x0;
		float dy = y1 - y0;
		float xi = 1;
		if (dx < 0) {
			xi = -1;
			dx = -dx;
		}
		float D = 2 * dx - dy;
		float x = x0;

		for (float y = y0; y <= y1; y += 1) {
			writePixel(x, y, nivel);
			if (D > 0) {
				x = x + xi;
				D = D - 2 * dy;
			}
			D = D + 2 * dx;
		}
	}

	void plotLine(Point p0, Point p1) {
		glColor3f(1, 0.1, 0.1);
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex2f(p0.getX() * x_ratio + bottom.getX(),
			p0.getY()*y_ratio + bottom.getY()
			);
		glVertex2f(p1.getX() * x_ratio + bottom.getX(),
			p1.getY()*y_ratio + bottom.getY()
		);
		glEnd();

		float y1 = p1.getY();
		float y0 = p0.getY();
		float x0 = p0.getX();
		float x1 = p1.getX();

		if (fabs(y1 - y0) < fabs(x1 - x0)) {
			if (x0 > x1)
				plotLineLow(x1, y1, x0, y0, 0);
			else
				plotLineLow(x0, y0, x1, y1, 0);
		}
		else {
			if (y0 > y1)
				plotLineHigh(x1, y1, x0, y0, 0);
			else
				plotLineHigh(x0, y0, x1, y1, 0);
		}
	}
};

int size = 16;
Grill G(size);
Point p1, p2, p3, p4;

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(2);

	glPointSize(3);

	glPolygonMode(GL_FRONT, GL_FILL);
}


void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	G.resize(height, width);
	G.draw();
	p1.setX(0);
	p1.setY(15);
	p2.setX(15);
	p2.setY(10);
	G.plotLine(p1, p2);

	p3.setX(0);
	p3.setY(0);
	p4.setX(15);
	p4.setY(7);
	G.plotLine(p3, p4);

	glFlush();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	height = h;
	width = w;
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay(); 
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(DEFAULT_SIZE, DEFAULT_SIZE);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}