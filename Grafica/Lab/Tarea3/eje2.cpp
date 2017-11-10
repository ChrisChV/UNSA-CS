#include <GL/glut.h>
#include <iostream>
#include <cmath>


GLsizei winWidth = 600, winHeight = 500;

struct Point{
    GLint x;
    GLint y;
};

void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

void drawPoint(Point p){
    glBegin(GL_POINTS);
        glVertex2i(p.x, p.y);
    glEnd();
}

void drawLine(Point a, Point b){
    glBegin(GL_LINES);
        glVertex2i(a.x, a.y);
        glVertex2i(b.x, b.y);
    glEnd();
}

void DDA(Point ini, Point final){
    GLint deltaX = final.x - ini.x;
    GLint deltaY = final.y - ini.y;
    GLint pasos = -1;
    GLint incrementoX = -1;
    GLint incrementoY = -1;
    Point actual;
    if(abs(deltaX) > abs(deltaY)) pasos = abs(deltaX);
    else pasos = abs(deltaY);
    incrementoX = deltaX / pasos;
    incrementoY = deltaY / pasos;
    actual.x = ini.x;
    actual.y = ini.y;
    drawPoint(actual);
    for(int i = 0; i < pasos; i++){
        actual.x += incrementoX;
        actual.y += incrementoY;
        drawPoint(actual);
    }
}

void display(){
    Point a;
    Point b;
    a.x = 50;
    a.y = 50;
    b.x = 400;
    b.y = 400;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    DDA(a,b);
    glFlush();

}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Dibujo de curvas");

    init();
    glutDisplayFunc(display);


    glutMainLoop();

}