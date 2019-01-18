#include <GL/glut.h>
#include "primitivas.h"

Window window;

Point origen;

void init(){
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,600.0,0,600.0);
	glMatrixMode(GL_MODELVIEW);
}

Point convertirPunto(Point punto){
	int despl = 20;
	Point res;
	res.x = origen.x + despl * punto.x;
	res.y = origen.y + despl * punto.y;
	return res;
}

void displayFcn(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	Point ejeX1; ejeX1.x = 20; ejeX1.y = 300;
	Point ejeX2; ejeX2.x = 580; ejeX2.y = 300;
	Point ejeY1; ejeY1.x = 300; ejeY1.y = 20;
	Point ejeY2; ejeY2.x = 300; ejeY2.y = 580;
	drawLineXnpio(ejeX1, ejeX2, window);
	drawLineXnpio(ejeY1, ejeY2, window);
	glBegin(GL_POINTS);	
		glVertex2f(origen.x, origen.y);
	glEnd();
	Point A; A.x = 5; A.y = 6;
	Point B; B.x = 7; B.y = 8;
	Point C; C.x = 3; C.y = 7;
	Point A_2 = convertirPunto(A);
	Point B_2 = convertirPunto(B);
	Point C_2 = convertirPunto(C);
	changeColor(ROJO);
	drawTriangle(A_2, B_2, C_2, window);
	Point eje; eje.x = 2; eje.y = 2;
	A.x += eje.x; A.y += eje.y;
	B.x += eje.x; B.y += eje.y;
	C.x += eje.x; B.y += eje.y;
	A_2 = convertirPunto(A);
	B_2 = convertirPunto(B);
	C_2 = convertirPunto(C);
	changeColor(AZUL);
	drawTriangle(A_2, B_2, C_2, window);
	A = rotatePoint(A, eje, 90);
	B = rotatePoint(B, eje, 90);
	C = rotatePoint(C, eje, 90);
	A_2 = convertirPunto(A);
	B_2 = convertirPunto(B);
	C_2 = convertirPunto(C);
	changeColor(VIOLETA);
	drawTriangle(A_2, B_2, C_2, window);
	A.y = -A.y;
	B.y = -B.y;
	C.y = -C.y;
	A_2 = convertirPunto(A);
	B_2 = convertirPunto(B);
	C_2 = convertirPunto(C);
	changeColor(MAGENTA);
	drawTriangle(A_2, B_2, C_2, window);
	glFlush();
}

int main(int argc, char ** argv){
	origen.x = 300; origen.y = 300;
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