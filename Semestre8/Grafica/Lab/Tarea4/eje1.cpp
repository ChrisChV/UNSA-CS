#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>

int windowWidth;
int windowHeight;

int xi, xf, xt;
int yi, yf, yt;

void init(){
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void display(){
	int i;
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(192, 192, 192);
	glBegin(GL_POINTS);
	for(i = xi; i <= xf; i++)
		glVertex2i(i, yi), glVertex2i(i, yf);
	for(i = yi; i <= yf; i++)
		glVertex2i(xi, i), glVertex2i(xf, i);
	for(i = xi; i >= xf; i--)
		glVertex2i(i, yi), glVertex2i(i, yf);
	for(i = yi; i >= yf; i--)
		glVertex2i(xi, i), glVertex2i(xf, i);
	glEnd();

	glBegin(GL_POINTS);
	glColor3ub(228, 0, 0);
	for(i =- 10; i <= 10;	 i++)
		glVertex2i(xt + i, yt + i), glVertex2i(xt+i, yt-i);
	glColor3ub(0, 228, 0);
	for(i =- 10; i <= 10; i++)
		glVertex2i(xi + i, yi), glVertex2i(xi, yi + i);
	glEnd();
	glutSwapBuffers();

}

void resize(GLsizei w, GLsizei h){
	windowWidth = w;
	windowHeight = h;
	glViewport(0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, 1.f, -1.f);
	printf("Resize: Nuevo tam de ventana: %dx%d\n", w, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse_move(int x, int y){
	xt = x;
	yt = windowHeight - y;
	glutPostRedisplay();
}

void mouse_drag(int x, int y){
	xt = x;
	yt = windowHeight - y;
	glutPostRedisplay();
}

typedef GLubyte Color[3];

void getPixel(int x, int y, Color rgb){
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
}

void mouse_click(int button, int state, int x, int y){
	Color c;
	if(state == GLUT_DOWN){
		xi = x;
		yi = windowHeight - y;
		xf = xi;
		yf = yi;
	}
	getPixel(xi, yi, c);
	if(state == GLUT_UP){

	}
	glutPostRedisplay();
}

void normal_key(GLubyte key, int x, int y){
	char s[80] = "";
	if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) sprintf(s, "%s SHIFT", s);
	if(glutGetModifiers() & GLUT_ACTIVE_CTRL) sprintf(s, "%s CTRL", s);
	if(glutGetModifiers() & GLUT_ACTIVE_ALT) sprintf(s, "%s ALT", s);
	//if(key >= 32 && key <= 128)
	if(key == 27) exit(0);
}

void special_key(int key, int x, int y){
	printf("Special key: %d \n", key);
}

void timer_function(int value){
	glutTimerFunc(8000, timer_function, 1);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("GL Template Basic");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutPassiveMotionFunc(mouse_move);
	glutMotionFunc(mouse_drag);
	glutMouseFunc(mouse_click);
	glutKeyboardFunc(normal_key);
	glutSpecialFunc(special_key);
	glutTimerFunc(33, timer_function, 1);

	init();
	glutMainLoop();
	return 0;
}