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
	glColor3f(1.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex2i(10, 10);
		glVertex2i(100, 10);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2i(150, 50);
		glVertex2i(100, 100);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2i(50, 80);
		glVertex2i(10, 10);
	glEnd();
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