#include <GL/glut.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;


void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	GLubyte fly[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x03, 0x80, 0x01, 0xc0, 0x06, 0xc0, 0x03, 0x60,
		0x04, 0x60, 0x06, 0x20, 0x04, 0x30, 0x0c, 0x20,
		0x04, 0x18, 0x18, 0x20, 0x04, 0x0c, 0x30, 0x20,
		0x04, 0x06, 0x60, 0x20, 0x44, 0x03, 0xc0, 0x22,
		0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
		0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
		0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
		0x66, 0x01, 0x80, 0x66, 0x33, 0x01, 0x80, 0xcc,
		0x19, 0x81, 0x81, 0x98, 0x0c, 0xc1, 0x83, 0x30,
		0x07, 0xe1, 0x87, 0xe0, 0x03, 0x3f, 0xfc, 0xc0,
		0x06, 0x64, 0x26, 0x60, 0x0c, 0xcc, 0x33, 0x30,
		0x18, 0xcc, 0x33, 0x18, 0x10, 0xc4, 0x23, 0x08,
		0x10, 0x63, 0xc6, 0x08, 0x10, 0x30, 0x0c, 0x08,
		0x10, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x08};
	GLubyte simple_pattern[] ={
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(25.0, 25.0, 125.0, 125.0);
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(fly);
	glBegin(GL_POLYGON);
		glVertex2i(125,25);
		glVertex2i(175,125);
		glVertex2i(225,25);
	glEnd();
	glPolygonStipple(simple_pattern);
	glColor3f(0.0f,0.0f,1.0f);
	glRectf(225.0, 25.0, 325.0, 125.0);
	glDisable(GL_POLYGON_STIPPLE);
	glFlush();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ejercicio 2");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}