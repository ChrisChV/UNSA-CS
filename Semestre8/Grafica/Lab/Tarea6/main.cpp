#include <GL/glut.h>
#include <iostream>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include "primitivas.h"

using namespace std;

GLsizei winWidth = 1200, winHeight = 800;
Window window;

Patron patron = {{0,0,0,0,0,0,0,0,0},
                 {0,1,0,0,0,0,0,1,0},
                 {0,1,1,0,0,0,1,1,0},
                 {0,1,0,1,0,1,0,1,0},
                 {0,1,0,0,1,0,0,1,0},
                 {0,1,0,1,0,1,0,1,0},
                 {0,1,1,0,0,0,1,1,0},
                 {0,1,0,0,0,0,0,1,0},
                 {0,0,0,0,0,0,0,0,0}};


void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 1200.0, 0.0, 800.0);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);	
    changeColor(ROJO);
    Point iniRec;
    iniRec.x = 150; iniRec.y = 150;
    Point finRec;
    finRec.x = 300; finRec.y = 300;
    Matrix rectangle = drawRectangle(iniRec, finRec, window);
    fillFigureWithPatron(rectangle, patron, AZUL);
    glFlush();
}


int main(int argc, char **argv){
    srand (time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    window.height = winHeight;
    window.width = winWidth;
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Programa Primitivas");
    init();
    glutDisplayFunc(display);


    glutMainLoop();
    return 0;
}




