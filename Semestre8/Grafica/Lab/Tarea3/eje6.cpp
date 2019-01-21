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

void drawPoint(Point p){
    glBegin(GL_POINTS);
        glVertex2i(p.x, p.y);
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


void bresenham(Point ini, Point fin){
    GLint dX = fin.x - ini.x;
    GLint dY = fin.y - ini.y;
    GLint IncYi;
    GLint IncXi;
    GLint IncXr;
    GLint IncYr;
    GLint avR;
    GLint av;
    GLint avI;
    if(dY >= 0) IncYi = 1;
    else{
        dY = -dY;
        IncYi = -1;
    }
    if(dX >= 0) IncXi = 1;
    else{
        dX = -dX;
        IncXi = -1;
    }
    if(dX >= dY){
        IncYr = 0;
        IncXr = IncXi;
    }
    else{
        IncXr = 0;
        IncYr = IncYi;
        swap(dX,dY);
    }
    Point actual;
    actual.x = ini.x;
    actual.y = ini.y;
    avR = 2 * dY;
    av = avR - dX;
    avI = av - dX;
    while(actual.x != fin.x){
        drawPoint(actual);
        if(av >= 0){
            actual.x += IncXi;
            actual.y += IncYi;
            av += avI;
        }
        else{
            actual.x += IncXr;
            actual.y += IncYr;
            av += avR;
        }
    }
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


void arte(){
	Point rec1;
	Point rec2;
	Point rec3;
	Point rec4;
	Point centro;
	centro.x = winWidth / 2;
	centro.y = winHeight / 2;
	GLint desplRec = 5;
	GLint finalRec = 50;
	glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    for(GLint i = desplRec; i < finalRec; i += desplRec){
    	rec1.x = i; rec1.y = i;
    	rec2.x = i; rec2.y = winHeight - i;
    	rec3.x = winWidth - i; rec3.y = winHeight - i;
    	rec4.x = winWidth - i; rec4.y = i;	
    	DDA(rec1, rec2);
		bresenham(rec2, rec3);
		DDA(rec3, rec4);
		bresenham(rec1, rec4);
    }
    GLint desplCirc = 5;
    GLint finalCirc = 50;
    GLint desplXElip = 5;
    GLint desplYElip = 1;
    GLint finalElip = 50;
    centro.x -= 200;
    for(int k = 0; k < 5; k++){
    	circleMidPoint(centro, 1);
    	for(GLint i = desplCirc; i < finalCirc; i += desplCirc){
    		circleMidPoint(centro, i);
    	}
    	centro.x += 100;
    }
    
    centro.x = winWidth / 2;
    centro.y -= 100;
    GLint rX = 1;
    GLint rY = 2;
    while(rX != finalElip and rY != finalElip){
    	elipceMidPoint(centro, rX, rY);
    	rX += desplXElip;
    	rY += desplYElip;
    }
    rX -= desplXElip;
    rY -= desplYElip;
    centro.y += 200;
    elipceMidPoint(centro, rX, rY);
    circleMidPoint(centro, rY);

    for(int i = rY / 2; i != 5; i--){
    	circleMidPoint(centro, i);	
    }
	glFlush();
}


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("ARTE");

    init();
    glutDisplayFunc(arte);


    glutMainLoop();

}




