#include <GL/glut.h>

class wcPt2D{
public:
	GLfloat x,y;
};

void init(){
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,600,0,300);
	glMatrixMode(GL_MODELVIEW);
}

void triangle(wcPt2D * verts){
	GLint k;
	glBegin(GL_TRIANGLES);
		for(k = 0; k < 3; k++){
			glVertex2f(verts[k].x, verts[k].y);
		}
	glEnd();
}

void displayFcn(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,1.0);
	glRecti(50,100,200,150);
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(200, 50, 0);
	glRecti(50,100,200,150);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90.0,0.0,0.0,1.0);
	glTranslatef(70,-600,0);
	glRecti(50,100,200,150);
	glPopMatrix();
	glPushMatrix();
	glScalef(0.5,1.0,1.0);
	glTranslatef(500,-50,0);
	glRecti(50,100,200,150);
	glPopMatrix();
	glPushMatrix();
	glFlush();
}

int main(int argc, char ** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(600,300);
	glutCreateWindow("Ejemplo splot - Screen");
	init();
	glutDisplayFunc(displayFcn);
	glutMainLoop();
	return 0;
}