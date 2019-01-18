#include <GL/glut.h>

int iFondo = 0;
int iDibujo = 3;

typedef enum {FONDO1, FONDO2, FONDO3, FONDO4, FONDO5, DIBUJO1, DIBUJO2, DIBUJO3, DIBUJO4} opcionesMenu;

void onMenu(int option){
	switch(option){
		case FONDO1: iFondo = 0; break;
		case FONDO2: iFondo = 1; break;
		case FONDO3: iFondo = 2; break;
		case DIBUJO1: iDibujo = 3; break;
		case DIBUJO2: iDibujo = 4; break;
		case DIBUJO3: iDibujo = 5; break;
	}
	glutPostRedisplay();
}

void creationMenu(){
	int menuFondo, menuDibujo, menuPrincipal;
	menuFondo = glutCreateMenu(onMenu);
	glutAddMenuEntry("Negro", FONDO1);
	glutAddMenuEntry("Verde Oscuro", FONDO2);
	glutAddMenuEntry("Azul oscuro", FONDO3);

	menuDibujo = glutCreateMenu(onMenu);
	glutAddMenuEntry("Blanco", DIBUJO1);
	glutAddMenuEntry("Verde Claro", DIBUJO2);
	glutAddMenuEntry("Acul clarro", DIBUJO3);

	menuPrincipal = glutCreateMenu(onMenu);
	glutAddSubMenu("Color de fondo", menuFondo);
	glutAddSubMenu("Color de dibujo", menuDibujo);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display(){
	float colores[6][3] = {
		{ 0.00f, 0.00f, 0.00f},
		{ 0.06f, 0.25f, 0.13f},
		{ 0.10f, 0.07f, 0.33f},
		{ 1.00f, 1.00f, 1.00f},
		{ 0.12f, 0.50f, 0.26f},
		{ 0.20f, 0.14f, 0.66f}
	};

	glClearColor(colores[iFondo][0], colores[iFondo][1], colores[iFondo][2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(colores[iDibujo][0], colores[iDibujo][1], colores[iDibujo][2]);
	glutWireTeapot(0.5);
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ejemplo de menus");
	glutDisplayFunc(display);
	creationMenu();
	glutMainLoop();
	return 0;
}