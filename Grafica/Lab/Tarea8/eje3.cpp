#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <tuple>

using namespace std;

GLfloat color[5][3] =
{
	{1.0,0.0,0.0},
	{1.0,1.0,0.0},
	{0.0,1.0,0.0},
	{0.0,0.0,1.0},
	{1.0,0.0,1.0},
};

void drawCilindro(float cx, float cy, float r, float z,  int num_segments) 
{ 
	float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = r;
	float y = 0; 
	vector<tuple<float,float,float>> puntos;
	glColor3fv(color[1]);
	glBegin(GL_POLYGON);
		for(int i = 0; i < num_segments; i++) 
		{ 
			puntos.push_back(make_tuple(x + cx, y + cy, z));
			glVertex3f(x + cx, y + cy, z);
			float tx = -y; 
			float ty = x; 
			x += tx * tangetial_factor; 
			y += ty * tangetial_factor;  
			x *= radial_factor; 
			y *= radial_factor;
		} 
	glEnd(); 
	glBegin(GL_POLYGON);
		for(int i = 0; i < puntos.size(); i++){
			tie(x,y,z) = puntos[i];
			z += 2;
			glVertex3f(x,y,z);
		}
	glEnd();
	glColor3fv(color[2]);
	float antX, antY, antZ;
	float actualX, actualY, actualZ;
	tie(antX,antY,antZ) = puntos[0];
	for(int i = 1; i < puntos.size(); i++){
		tie(actualX, actualY, actualZ) = puntos[i];
		glBegin(GL_QUADS);
			glVertex3f(antX,antY,antZ);
			glVertex3f(antX,antY,antZ + 2);
			glVertex3f(actualX, actualY, actualZ + 2);
			glVertex3f(actualX, actualY, actualZ);
		glEnd();
		antX = actualX;
		antY = actualY;
		antZ = actualZ;
	}
	tie(actualX, actualY, actualZ) = puntos[0];
	glBegin(GL_QUADS);
		glVertex3f(antX,antY,antZ);
		glVertex3f(antX,antY,antZ + 2);
		glVertex3f(actualX, actualY, actualZ + 2);
		glVertex3f(actualX, actualY, actualZ);
	glEnd();
}

double rotate_y = 0;
double rotate_x = 0;

void specialKeys(int key, int x, int y){
	if(key == GLUT_KEY_RIGHT) rotate_y += 5;
	else if(key == GLUT_KEY_LEFT) rotate_y -=5;
	else if(key == GLUT_KEY_UP) rotate_x +=5;
	else if(key == GLUT_KEY_DOWN) rotate_x -= 5;
	glutPostRedisplay();
}
void display(){
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(60, w/h, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3,3,3,
			  0,0,0,
			  0,0,1);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	drawCilindro(0,0,1,-1.0, 100);
	glutSwapBuffers();
}

int main(int argc, char ** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("GLUT");
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}