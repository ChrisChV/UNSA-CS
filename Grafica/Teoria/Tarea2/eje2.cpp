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

void sapito(){
    Point centro;
    Point bocaDer;
    Point bocaIzq;
    Point bocaCurIzq;
    Point bocaCurDer;
    Point centroOjo;
    centro.x = winWidth / 2;
    centro.y = winHeight / 2;
    GLint radioCabezaY = 100;
    GLint radioCabezaX = 150;
    GLint radioOjo = 45;
    GLint radioOjito = 25;
    GLint distanciaAlOjo = 150;
    GLint anguloOjo = 80;
    GLint distanciaBoca = 110;
    GLint tamBoca = 90;
    GLint desplOjoX = distanciaAlOjo * sin(anguloOjo / 2 * PI / 180);
    GLint desplOjoY = distanciaAlOjo * cos(anguloOjo / 2 * PI / 180);

    Matrix cabeza = elipceMidPoint(centro, radioCabezaX, radioCabezaY, window);
    
    centroOjo.x = centro.x - desplOjoX;
    centroOjo.y = centro.y + desplOjoY;
    bocaDer.x = centroOjo.x;
    bocaDer.y = centroOjo.y - distanciaBoca;

    Matrix ojoDerecho = circleMidPoint(centroOjo, radioOjo, window);
    fillFigure(ojoDerecho, VERDE);
    glColor3f(0.0, 0.0, 0.0);
    Matrix ojitoDerecho = circleMidPoint(centroOjo, radioOjito, window);
    fillFigure(ojitoDerecho, BLANCO);

    centroOjo.x = centro.x + desplOjoX;
    centroOjo.y = centro.y + desplOjoY;
    bocaIzq.x = centroOjo.x;
    bocaIzq.y = centroOjo.y - distanciaBoca;

    glColor3f(0.0, 0.0, 0.0);
    Matrix ojoIzquierdo = circleMidPoint(centroOjo, radioOjo, window);
    fillFigure(ojoIzquierdo, VERDE);    
    glColor3f(0.0, 0.0, 0.0);
    Matrix ojitoIzquierdo = circleMidPoint(centroOjo, radioOjito, window);
    fillFigure(ojitoIzquierdo, BLANCO);
    fillFigure(cabeza, VERDE);

    glColor3f(0.0, 0.0, 0.0);
    
    bocaCurIzq.x = bocaIzq.x;
    bocaCurIzq.y = bocaIzq.y - tamBoca;
    bocaCurDer.x = bocaDer.x;
    bocaCurDer.y = bocaDer.y - tamBoca;
    Matrix lineaBoca = drawLineXnpio(bocaIzq, bocaDer, window);
    Matrix curvaBoca = bezier(bocaIzq, bocaDer, bocaCurIzq, bocaCurDer, window);
    Matrix boca = lineaBoca + curvaBoca;
    fillFigure(boca, BLANCO);
}

void display(){
    

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    sapito();
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

