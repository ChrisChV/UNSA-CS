#include <GL/glut.h>
#include <cmath>

#define PI 3.14159265

struct Point{
	int x;
	int y;
};

void rotatePoint(Point &punto, Point eje, float grados){
    Point res;
    float coseno = cos(grados*PI/180.0);
    float seno = sin(grados*PI/180.0);
    res.x = coseno * punto.x - seno * punto.y + eje.x * (1 - coseno) + eje.y * seno;
    res.y = seno * punto.x + coseno * punto.y + eje.y * (1 - coseno) - eje.x * seno;
    punto = res;
}

void init(){
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-300,300,-300,300);
	glMatrixMode(GL_MODELVIEW);
}

void drawImage(Point a, Point b, Point c, Point d, Point e, Point f){
	glBegin(GL_LINE_LOOP);
		glVertex2f(a.x, a.y);
		glVertex2f(b.x, b.y);
		glVertex2f(c.x, c.y);
		glVertex2f(d.x, d.y);
		glVertex2f(e.x, e.y);
		glVertex2f(f.x, f.y);
	glEnd();
}

void scalePoint(Point &a, float sX, float sY){
	a.x *= sX;
	a.y *= sY;
}

void displayFcn(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	glViewport(0,300,300,300);
	Point a, b, c , d, e, f;
	Point tA, tB, tC, tD, tE, tF;
	a.x = 100; a.y = 150;
	b.x = 100; b.y = 100;
	c.x = 150; c.y = 80;
	d.x = 200; d.y = 100;
	e.x = 200; e.y = 150;
	f.x = 150; f.y = 120;
	tA = a; tB = b; tC = c; tD = d; tE = e; tF = f;
	drawImage(a, b, c, d, e, f);
	glViewport(300,300,300,300);
	scalePoint(tA, 0.5, 0.5);
	scalePoint(tB, 0.5, 0.5);
	scalePoint(tC, 0.5, 0.5);
	scalePoint(tD, 0.5, 0.5);
	scalePoint(tE, 0.5, 0.5);
	scalePoint(tF, 0.5, 0.5);
	drawImage(tA, tB, tC, tD, tE, tF);
	glViewport(0,0,300,300);
	tA = a; tB = b; tC = c; tD = d; tE = e; tF = f;
	tA.y = -tA.y;
	tB.y = -tB.y;
	tC.y = -tC.y;
	tD.y = -tD.y;
	tE.y = -tE.y;
	tF.y = -tF.y;

	drawImage(tA, tB, tC, tD, tE, tF);
	glLoadIdentity();
	glViewport(300,0,300,300);
	tA = a; tB = b; tC = c; tD = d; tE = e; tF = f;
	Point eje; eje.x = 0; eje.y = 0;
	rotatePoint(tA, eje, -30.0);
	rotatePoint(tB, eje, -30.0);
	rotatePoint(tC, eje, -30.0);
	rotatePoint(tD, eje, -30.0);
	rotatePoint(tE, eje, -30.0);
	rotatePoint(tF, eje, -30.0);
	drawImage(tA, tB, tC, tD, tE, tF);

	glFlush();
}

int main(int argc, char ** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(600,600);
	glutCreateWindow("Ejemplo splot - Screen");
	init();
	glutDisplayFunc(displayFcn);
	glutMainLoop();
	return 0;
}