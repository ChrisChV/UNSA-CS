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

void elipcePlotPoints(Point centro, Point actual){
    setPixel(centro.x + actual.x, centro.y + actual.y);
    setPixel(centro.x - actual.x, centro.y + actual.y);
    setPixel(centro.x + actual.x, centro.y - actual.y);
    setPixel(centro.x - actual.x, centro.y - actual.y);
}

void elipceMidPoint(Point centro, GLint radiusX, GLint radiusY){
    GLdouble p1, p2;
    GLint rX2, rY2;
    Point actual;
    actual.x = 0;
    actual.y = radiusY;
    rX2 = pow(radiusX,2);
    rY2 = pow(radiusY,2);
    p1 = rY2 - (rX2 * radiusY) + (0.25 * rX2);
    while((rY2 * actual.x) < (rX2 * actual.y)){
        actual.x++;
        if(p1 < 0) p1 += (2 * rY2 * actual.x) + rY2;
        else{
            actual.y--;
            p1 += (2 * rY2 * actual.x) - (2 * rX2 * actual.y) + rY2;
        }
        elipcePlotPoints(centro,actual);
    }
    p2 = (rY2) * pow((actual.x + 0.5), 2) + (rX2) * pow((actual.y - 1), 2) - (rX2 * rY2);
    while(actual.y > 0){
        actual.y--;
        if(p2 > 0) p2 -= (2 * rX2 * actual.y) + rX2;
        else{
            actual.x++;
            p2 += (2 * rY2 * actual.x) - (2 * rX2 * actual.y) + rX2;
        }
        elipcePlotPoints(centro,actual);
    }
}

void display(){
    Point center;
    center.x = 300;
    center.y = 250;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    elipceMidPoint(center, 50, 200);
    elipceMidPoint(center, 200, 50);

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
