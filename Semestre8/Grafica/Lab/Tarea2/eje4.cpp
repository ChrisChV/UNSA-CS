#include <GL/glut.h>

GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;
int months = 6;
GLubyte label[18] = {'J','u','l','A','u','g','S','e','p',
'O','c','t','N','o','v','D','e','c'};
GLint dataValue[6] = {180,190,220,250,330,450};

void init(void){
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

void bartChart(void){
	GLint month, k;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0,0.0,0.0);
	for(k = 0; k < months; k++){
		glRecti(20 + k * 50, 165, 40 + k * 50, dataValue[k]);
	}
	glColor3f(0.0, 0.0, 0.0);
	xRaster = 20;
	for(month = 0; month < months; month++){
		glRasterPos2i(xRaster,yRaster);
		for(k = 3*month; k < 3*month + 3; k++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,label[k]);
		}
		xRaster += 50;
	}


	GLint x = 30;
    glColor3f(0.0, 0.0,1.0);
    glBegin(GL_LINE_STRIP);
        for(k = 0; k < months; k++){
            glVertex2i(x + k*50, dataValue[k]);
        }
    glEnd();
	xRaster = 25;
    for(k = 0; k < months; k++){
        glRasterPos2i(xRaster + k*50,dataValue[k] - 4);
        glutBitmapCharacter (GLUT_BITMAP_9_BY_15,'*');
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


