#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

#define PI 3.14159265

class srcPt{
public:
    GLint x, y;
};

GLsizei winWidth = 500, winHeight = 450;

void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,500.0,0.0,450.0);
}

void setPixel(GLint x, GLint y){
    glBegin(GL_POINTS);
        glVertex2i(x,y);
    glEnd();
}

void drawLine(srcPt pt1, srcPt pt2){
    glBegin(GL_LINES);
        glVertex2i(pt1.x,pt1.y);
        glVertex2i(pt2.x,pt2.y);
    glEnd();
}

void circlePlotPoints(srcPt circCtr, srcPt circPt){
    setPixel(circCtr.x + circPt.x,circCtr.y + circPt.y);
    setPixel(circCtr.x - circPt.x,circCtr.y + circPt.y);
    setPixel(circCtr.x + circPt.x,circCtr.y - circPt.y);
    setPixel(circCtr.x - circPt.x,circCtr.y - circPt.y);
    setPixel(circCtr.x + circPt.y,circCtr.y + circPt.x);
    setPixel(circCtr.x - circPt.y,circCtr.y + circPt.x);
    setPixel(circCtr.x + circPt.y,circCtr.y - circPt.x);
    setPixel(circCtr.x - circPt.y,circCtr.y - circPt.x);
}

void circleMidPoint(srcPt circCtr, GLint radius){
    srcPt circPt;
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

GLdouble getAngle(GLfloat value, GLfloat total){
    GLdouble porcent = (value * 100) / total;
    return (porcent * 360) / 100;
}

void pieChart(void){
    srcPt circCtr, piePt;
    GLint radius = winWidth / 4;
    GLdouble sliceAngle, previousSliceAngle = 0.0;
    GLint k, nSlices = 12;
    GLfloat dataValues[12] = {10.0,7.0,13.0,5.0,13.0,14.0,3.0,16.0,5.0,3.0,17.0,8.0};
    GLfloat total = 0.0;
    GLfloat dataSum = 0.0;
    srcPt secondPoint;
    circCtr.x = winWidth / 2;
    circCtr.y = winHeight / 2;
    for(int i = 0; i < nSlices; i++){
        total += dataValues[i];
    }
    for(int i = 0; i < nSlices; i++){
        dataSum += dataValues[i];
        sliceAngle = getAngle(dataSum, total);
        secondPoint.x = circCtr.x + cos(sliceAngle * PI/180) * radius;
        secondPoint.y = circCtr.y + sin(sliceAngle * PI/180) * radius;
        drawLine(circCtr,secondPoint);
    }
    circleMidPoint(circCtr, radius);
}

void displayFcn(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,1.0);
    pieChart();
    glFlush();
}


int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Pie Chart");
    init();
    glutDisplayFunc(displayFcn);
    glutMainLoop();

    return 0;
}
