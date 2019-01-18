#include <GL/glut.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 1200, winHeight = 800;
int primitiva = -1;

enum Primitivas {POINTS = 1, LINES, STRIP, LINE_LOOP, POLYGON, TRIANGLE_STRIP, TRIANGLES, TRIANGLE_FAN, QUADS, QUAD_STRIP};

void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 1200.0, 0.0, 800.0);
}

void drawString(string s, int x, int y){
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(x,y);
	for(char c : s){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,c);
	}
	glColor3f(1.0, 0.0, 0.0);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
    	glVertex2i(70,770);
    	glVertex2i(90,770);
    	glVertex2i(80,750);
    	glVertex2i(90,760);
    glEnd();
    drawString("GL_POINTS", 50, 730);
    glBegin(GL_LINES);
    	glVertex2i(200, 730);
    	glVertex2i(250, 800);
    	glVertex2i(240, 760);
    	glVertex2i(270, 730);
    glEnd();
    drawString("GL_LINES", 205, 700);
    glBegin(GL_LINE_STRIP);
    	glVertex2i(500, 700);
    	glVertex2i(400, 780);
    	glVertex2i(350, 740);
    	glVertex2i(600, 760);
    	glVertex2i(500, 780);
    	glVertex2i(560, 670);
    	glVertex2i(450, 650);
    glEnd();
    drawString("GL_LINE_STRIP", 450, 620);
    glBegin(GL_LINE_LOOP);
    	glVertex2i(700, 750);
    	glVertex2i(800, 790);
    	glVertex2i(750, 780);
    	glVertex2i(750, 730);
    	glVertex2i(790, 720);
    	glVertex2i(740, 670);
    glEnd();
    drawString("GL_LINE_LOOP", 700, 640);
    glBegin(GL_POLYGON);
    	glVertex2i(900, 750);
    	glVertex2i(950, 800);
    	glVertex2i(1000, 750);
    	glVertex2i(1000, 700);
    	glVertex2i(950, 650);
    	glVertex2i(900, 700);
    glEnd();
    drawString("GL_POLYGON", 910, 620);
    glBegin(GL_TRIANGLE_STRIP);
    	glVertex2i(100, 550);
    	glVertex2i(250, 540);
    	glVertex2i(130, 450);
    	glColor3f(0.0, 1.0, 0.0);
    	glVertex2i(220, 460);
    	glVertex2i(110, 400);
    	glColor3f(0.0, 0.0, 1.0);
    	glVertex2i(210, 390);
    	glVertex2i(150, 350);
    	glColor3f(0.0, 0.0, 0.0);
    	glVertex2i(220, 360);
    glEnd();
    drawString("GL_TRIANGLE_STRIP", 120, 320);
    glBegin(GL_TRIANGLES);
    	glVertex2i(350, 500);
    	glVertex2i(400, 550);
    	glVertex2i(420, 490);
    	glVertex2i(460, 470);
    	glVertex2i(480, 520);
    	glVertex2i(550, 470);
    glEnd();
    drawString("GL_TRIANGLES", 390, 450);
    glBegin(GL_TRIANGLE_FAN);
    	glVertex2i(400, 340);
    	glVertex2i(420, 400);
    	glVertex2i(500, 405);
    	glColor3f(0.0, 1.0, 0.0);
    	glVertex2i(520, 370);
    	glColor3f(0.0, 0.0, 1.0);
    	glVertex2i(570, 350);
    	glColor3f(0.0, 0.0, 0.0);
    	glVertex2i(520, 320);
    glEnd();
    drawString("GL_TRIANGLE_FAN", 400, 300);
    glBegin(GL_QUADS);
    	glVertex2i(650, 520);
    	glVertex2i(660, 570);
    	glVertex2i(720, 550);
    	glVertex2i(690, 500);
    	glVertex2i(650, 450);
    	glVertex2i(730, 450);
    	glVertex2i(720, 420);
    	glVertex2i(630, 380);
    glEnd();
    drawString("GL_QUADS", 650, 350);
    glBegin(GL_QUAD_STRIP);
    	glVertex2i(800, 570);
    	glVertex2i(900, 580);
    	glVertex2i(820, 500);
    	glVertex2i(880, 510);
    	glColor3f(0.0, 1.0, 0.0);
    	glVertex2i(810, 460);
    	glVertex2i(875, 440);
    	glColor3f(0.0, 0.0, 1.0);
    	glVertex2i(850, 400);
    	glVertex2i(900, 420);
    glEnd();
    drawString("GL_QUAD_STRIP", 800, 380);

    glFlush();
}

/*
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    switch(primitiva){
        case POINTS: {
            glBegin(GL_POINTS);
                glVertex2i(250,250);
                glVertex2i(100,100);
                glVertex2i(50,50);
                glVertex2i(150,100);
            glEnd();
            break;
        }
        case LINES: {
            glBegin(GL_LINES);
                glVertex2i(250,250);
                glVertex2i(100,100);
                glVertex2i(50,50);
                glVertex2i(150,100);
                glVertex2i(500,400);
                glVertex2i(300,250);
            glEnd();
            break;
        }
        case STRIP: {
            glBegin(GL_LINE_STRIP);
                glVertex2i(250,250);
                glVertex2i(200,300);
                glVertex2i(150,270);
                glVertex2i(450,276);
                glVertex2i(300,300);
                glVertex2i(330,100);
                glVertex2i(170,80);
            glEnd();
            break;
        }
        case LINE_LOOP: {
            glBegin(GL_LINE_LOOP);
                glVertex2i(200, 200);
                glVertex2i(250, 50);
                glVertex2i(300, 150);
                glVertex2i(240, 170);
                glVertex2i(230, 230);
                glVertex2i(300, 230);
            glEnd();
            break;
        }
        case POLYGON: {
            glBegin(GL_POLYGON);
                glVertex2i(200, 200);
                glVertex2i(250, 250);
                glVertex2i(300, 200);
                glVertex2i(300, 150);
                glVertex2i(250, 100);
                glVertex2i(200, 150);
            glEnd();
            break;
        }
        case TRIANGLES: {
            glBegin(GL_TRIANGLES);
                glVertex2i(150,150);
                glVertex2i(200,200);
                glVertex2i(250,140);
                glVertex2i(400,400);
                glVertex2i(360,350);
                glVertex2i(500,300);
            glEnd();
            break;
        }
        case TRIANGLE_STRIP: {
            glBegin(GL_TRIANGLE_STRIP);
                glVertex2i(100,400);                
                glVertex2i(400,400);
                glVertex2i(130,300);
                glColor3f(0.0, 1.0, 0.0);
                glVertex2i(370,300);
                glVertex2i(90,250);
                glColor3f(0.0, 0.0, 1.0);
                glVertex2i(350,230);
                glVertex2i(150,150);
            glEnd();
            break;
        }
        case TRIANGLE_FAN: {
            glBegin(GL_TRIANGLE_FAN);
                glVertex2i(200,200);
                glVertex2i(220,250);
                glVertex2i(300,240);
                glColor3f(0.0, 1.0, 0.0);
                glVertex2i(330,230);
                glColor3f(0.0, 0.0, 1.0);
                glVertex2i(400,210);
                glVertex2i(350,230);
            glEnd();
            break;
        }
        case QUADS: {
            glBegin(GL_QUADS);
                glVertex2i(150,200);
                glVertex2i(100,150);
                glVertex2i(150,100);
                glVertex2i(200,150);
            glEnd();
            break;
        }
        case QUAD_STRIP: {
            glBegin(GL_QUAD_STRIP);
                glVertex2i(100,400);
                glVertex2i(400,380);
                glVertex2i(350,350);
                glVertex2i(150,350);
                
            glEnd();
        }
        default: break;
    }
    glFlush();
}
*/

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Programa Primitivas");
    init();
    /*
    cout<<"Seleccione primitiva a dibujar:"<<endl;
    cout<<"1->Puntos"<<endl;
    cout<<"2->Líneas"<<endl;
    cout<<"3->Conjunto de Líneas"<<endl;
    cout<<"4->Loop de Líneas"<<endl;
    cout<<"5->Poligono"<<endl;
    cout<<"6->Conjunto de Triangulos"<<endl;
    cout<<"7->Triangulos"<<endl;
    cout<<"8->Triangle Fan"<<endl;
    cout<<"9->Cuadrilateros"<<endl;
    cout<<"10->Conjunto de Cuadrilateros"<<endl;
    cout<<"Respuesta->";
    cin>>primitiva;
    */


    glutDisplayFunc(display);


    glutMainLoop();
    return 0;
}




