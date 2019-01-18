#include <GL/glut.h>

void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,0); glLineWidth(3);
	glBegin(GL_LINE_STRIP);
		glVertex2i(130,060); glVertex2i( 50,060);
		glVertex2i(130,150); glVertex2i( 50,150);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i( 70,100); glVertex2i(110,100);
		glVertex2i(150,100); glVertex2i(230,100);
		glVertex2i(190,140); glVertex2i(190,070);
		glVertex2i(250,100); glVertex2i(330,100);
		glVertex2i(290,140); glVertex2i(290,070);
	glEnd();
	glutSwapBuffers();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (640,480);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glClearColor(1.f,1.f,1.f,1.f);
}

int main (int argc, char **argv) {
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}

