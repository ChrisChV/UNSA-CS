#include <GL/glut.h>
#include "primitivas.h"

Window window;

void init(){
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,600.0,0,600.0);
	glMatrixMode(GL_MODELVIEW);
}

void displayFcn(){
	Point A; A.x = 200; A.y = 300;
	Point B; B.x = 400; B.y = 400;
	Point C; C.x = 400; C.y = 200;
	Point eje; eje.x = 300; eje.y = 300;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,1.0);
	glViewport(0,300,300,300);
	Matrix triangle1 = drawTriangle(A, B, C, window);
	fillFigureScanLine(triangle1, AZUL);
	A = rotatePoint(A, eje, 90);
	B = rotatePoint(B, eje, 90);
	C = rotatePoint(C, eje, 90);
	glViewport(0,0,300,300);
	Matrix triangle2 = drawTriangle(A, B, C, window);
	fillFigureScanLine(triangle2, AZUL);
	A = rotatePoint(A, eje, 90);
	B = rotatePoint(B, eje, 90);
	C = rotatePoint(C, eje, 90);
	glViewport(300,0,300,300);
	Matrix triangle3 = drawTriangle(A, B, C, window);
	fillFigureScanLine(triangle3, AZUL);
	A = rotatePoint(A, eje, 90);
	B = rotatePoint(B, eje, 90);
	C = rotatePoint(C, eje, 90);
	glViewport(300,300,300,300);
	Matrix triangle4 = drawTriangle(A, B, C, window);
	fillFigureScanLine(triangle4, AZUL);
	glFlush();
}

int main(int argc, char ** argv){
	window.height = 600;
	window.width = 600;
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