#include <GL/glut.h>

GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;
GLubyte label[36] = {'J','a','n','F','e','b','M','a','r',
'A','p','r','M','a','y','J','u','n','J','u','l','A','u',
'q','S','e','p','O','c','t','N','o','v','D','e','c'};
GLint dataValue[12] = {420,342,324,310,262,185,190,196,217,240,312,438};

void init(void){
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

void bartChart(void){
	GLint month, k;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0,0.0,0.0);
	for(k = 0; k < 12; k++){
		glRecti(20 + k * 50, 165, 40 + k * 50, dataValue[k]);
	}
	glColor3f(0.0, 0.0, 0.0);
	xRaster = 20;
	for(month = 0; month < 12; month++){
		glRasterPos2i(xRaster,yRaster);
		for(k = 3*month; k < 3*month + 3; k++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,label[k]);
		}
		xRaster += 50;
	}
	glFlush();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Line Chart Data Plor");

	init();
	glutDisplayFunc(bartChart);

	glutMainLoop();
	return 0;
}




