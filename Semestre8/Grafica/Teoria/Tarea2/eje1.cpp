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


void estrella(Point centro, GLint radio){
    Point estrella1;
    Point estrella2;
    Point estrella3;
    Point estrella4;
    Point estrella5;
    Point estrella6;
    GLint angulo = 36;
    GLint espacio = 5; // Espacio entre el circulo y la estrella
    GLdouble cuerda = radio * cos((angulo / 2) * PI / 180); // Distancia entre dos puntas opuestas de la estrella
    cuerda *= 2;
    GLdouble lado = (2 * cuerda - espacio * 4) / 5.0;
    

    estrella1.x = centro.x;
    estrella1.y = centro.y + radio - espacio;

    estrella2.x = estrella1.x + lado * sin((angulo / 2) * PI / 180);
    estrella2.y = estrella1.y - lado * cos((angulo / 2) * PI / 180);;

    estrella3.x = estrella2.x + lado - espacio;
    estrella3.y = estrella2.y;
    
    estrella4.x = estrella3.x - lado * cos(angulo * PI / 180);
    estrella4.y = estrella3.y - lado * sin(angulo * PI / 180);

    estrella5.x = estrella4.x + (lado - espacio * 3) * sin(angulo/ 2 * PI / 180);
    estrella5.y = estrella4.y - (lado - espacio * 3) * cos(angulo/ 2 * PI / 180);

    estrella6.x = centro.x;
    estrella6.y = estrella5.y + lado * sin(angulo * PI / 180);

    drawLineXnpio(estrella1, estrella2, window); // Mi algoritmo creado en una practica pasada
    drawLineXnpio(estrella2, estrella3, window);
    drawLineXnpio(estrella3, estrella4, window);
    drawLineXnpio(estrella4, estrella5, window);
    drawLineXnpio(estrella5, estrella6, window);

    estrella2.x -= abs(estrella1.x - estrella2.x) * 2;
    estrella3.x -= abs(estrella1.x - estrella3.x) * 2;
    estrella4.x -= abs(estrella1.x - estrella4.x) * 2;
    estrella5.x -= abs(estrella1.x - estrella5.x) * 2;

    drawLineXnpio(estrella1, estrella2, window);
    drawLineXnpio(estrella2, estrella3, window);
    drawLineXnpio(estrella3, estrella4, window);
    drawLineXnpio(estrella4, estrella5, window);
    drawLineXnpio(estrella5, estrella6, window);

    circleMidPoint(centro, radio, window);
}

void display(){
    Point centro;
    centro.x = winWidth / 2;
    centro.y = winHeight / 2;
    GLint radio = 200;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    estrella(centro, radio);
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

