#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

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

void setPixel(GLint x, GLint y){
    glBegin(GL_POINTS);
        glVertex2i(x,y);
    glEnd();
}


void circlePlotPoints(Point circCtr, Point circPt){
    setPixel(circCtr.x + circPt.x,circCtr.y + circPt.y);
    setPixel(circCtr.x - circPt.x,circCtr.y + circPt.y);
    setPixel(circCtr.x + circPt.x,circCtr.y - circPt.y);
    setPixel(circCtr.x - circPt.x,circCtr.y - circPt.y);
    setPixel(circCtr.x + circPt.y,circCtr.y + circPt.x);
    setPixel(circCtr.x - circPt.y,circCtr.y + circPt.x);
    setPixel(circCtr.x + circPt.y,circCtr.y - circPt.x);
    setPixel(circCtr.x - circPt.y,circCtr.y - circPt.x);
}

void circleMidPoint(Point circCtr, GLint radius){
    Point circPt;
    GLint p = 1 - radius;
    circPt.x = 0;
    circPt.y = radius;
    circlePlotPoints(circCtr, circPt);
    while(circPt.x < circPt.y){
        circPt.x++;
        if(p < 0) p += 2 * circPt.x + 1;
        else{
            circPt.y--;
            p += 2 * (circPt.x - circPt.y) + 1;
        }
        circlePlotPoints(circCtr, circPt);
    }
}

void display(){
    Point center;
    center.x = 300;
    center.y = 250;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    circleMidPoint(center, 200);
    glFlush();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Mid Point");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

}