#include <iostream>
#include "primitivas.h"

using namespace std;

#define PI 3.14159265

GLsizei winWidth = 600, winHeight = 500;
Window window;

void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}




void display(){
    Point centro;
    centro.x = winWidth / 2;
    centro.y = winHeight / 2;
    GLint radio = 200;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    Matrix circulo = elipceMidPoint(centro, radio, radio / 2, window);
    fillFigureScanLine(circulo, ROJO);
    glFlush();

}

int main(int argc, char **argv){
    window.width = winWidth;
    window.height = winHeight;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Dibujo");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}

