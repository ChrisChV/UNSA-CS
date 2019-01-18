#include <GL/glut.h>
#include <iostream>
#include <tuple>

using namespace std;

typedef tuple<float,float> Point;

float anchoPantalla;
float altoPantalla;
Point puntoCentralPisoCasa;
float anchoCasa;
float altoCasa;

float grosorCasa = 3;
float grosorTecho = 3;
float grosorPuerta = 1;
float grosorVentanas = 1;




void init(void){
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,anchoPantalla,0.0,altoPantalla);
}

void drawLine(Point a, Point b){
    float x1;
    float x2;
    float y1;
    float y2;
    tie(x1,y1) = a;
    tie(x2,y2) = b;
    glBegin(GL_LINES);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
    glEnd();
}


void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    float mitadAltoCasa = altoCasa / 2;
    float mitadAnchoCasa = anchoCasa / 2;
    float mitadMitadAnchoCasa = mitadAnchoCasa / 2;
    float mitadMitadAltoCasa = mitadAltoCasa / 2;
    float mitadMitadMitadAnchoCasa = mitadMitadAnchoCasa / 2;
    float mitadMitadMitadAltoCasa = mitadMitadAltoCasa / 2;
    float mitadMitadMitadMitadAltoCasa = mitadMitadMitadAnchoCasa / 2;
    float puntoCentralPisoCasaX;
    float puntoCentralPisoCasaY;
    tie(puntoCentralPisoCasaX,puntoCentralPisoCasaY) = puntoCentralPisoCasa;

    Point puntaTecho = make_tuple(puntoCentralPisoCasaX, puntoCentralPisoCasaY + altoCasa);
    Point paredSupIzq = make_tuple(puntoCentralPisoCasaX - mitadAnchoCasa,
        puntoCentralPisoCasaY + mitadAltoCasa);
    Point paredSupDer = make_tuple(puntoCentralPisoCasaX + mitadAnchoCasa,
        puntoCentralPisoCasaY + mitadAltoCasa);
    Point paredInfIzq = make_tuple(puntoCentralPisoCasaX - mitadAnchoCasa, 
        puntoCentralPisoCasaY);
    Point paredInfDer = make_tuple(puntoCentralPisoCasaX + mitadAnchoCasa, 
        puntoCentralPisoCasaY);

    glLineWidth(grosorCasa);
    glColor3f(1.0,0.0,0.0);
    drawLine(paredSupIzq,paredSupDer);
    drawLine(paredSupDer,paredInfDer);
    drawLine(paredInfDer,paredInfIzq);
    drawLine(paredInfIzq,paredSupIzq);
    glFlush();

    glLineWidth(grosorTecho);
    glColor3f(0.0,0.4,0.2);
    drawLine(paredSupIzq,puntaTecho);
    drawLine(paredSupDer,puntaTecho);
    glFlush();

    Point puertaSupIzq = make_tuple(puntoCentralPisoCasaX - mitadMitadAnchoCasa,
        puntoCentralPisoCasaY + mitadMitadAltoCasa);
    Point puertaSupDer = make_tuple(puntoCentralPisoCasaX + mitadMitadAnchoCasa,
        puntoCentralPisoCasaY + mitadMitadAltoCasa);
    Point puertaInfIzq = make_tuple(puntoCentralPisoCasaX - mitadMitadAnchoCasa,
        puntoCentralPisoCasaY);
    Point puertaInfDer = make_tuple(puntoCentralPisoCasaX + mitadMitadAnchoCasa,
        puntoCentralPisoCasaY);

    glColor3f(0.5,0.35,0.05);
    glLineWidth(grosorPuerta);
    drawLine(puertaSupDer,puertaSupIzq);
    drawLine(puertaSupIzq,puertaInfIzq);
    drawLine(puertaInfDer,puertaSupDer);
    glFlush();

    Point ventanaIzqSupIzq = make_tuple(puntoCentralPisoCasaX - (mitadMitadAnchoCasa + mitadMitadMitadAnchoCasa),
        puntoCentralPisoCasaY + mitadAltoCasa - mitadMitadMitadMitadAltoCasa);
    Point ventanaIzqSupDer = make_tuple(puntoCentralPisoCasaX - mitadMitadMitadAnchoCasa,
        puntoCentralPisoCasaY + mitadAltoCasa - mitadMitadMitadMitadAltoCasa);
    Point ventanaIzqInfDer = make_tuple(puntoCentralPisoCasaX - mitadMitadMitadAnchoCasa,
        puntoCentralPisoCasaY + mitadMitadAltoCasa + mitadMitadMitadMitadAltoCasa);
    Point ventanaIzqInfIzq = make_tuple(puntoCentralPisoCasaX - (mitadMitadAnchoCasa + mitadMitadMitadAnchoCasa),
        puntoCentralPisoCasaY + mitadMitadAltoCasa + mitadMitadMitadMitadAltoCasa);

    glColor3f(0.196078,0.6,0.8);
    glLineWidth(grosorVentanas);
    drawLine(ventanaIzqSupDer,ventanaIzqSupIzq);
    drawLine(ventanaIzqSupIzq,ventanaIzqInfIzq);
    drawLine(ventanaIzqInfIzq,ventanaIzqInfDer);
    drawLine(ventanaIzqInfDer,ventanaIzqSupDer);

    Point ventanaDerSupIzq = make_tuple(puntoCentralPisoCasaX + mitadMitadMitadAnchoCasa,
        puntoCentralPisoCasaY + mitadAltoCasa - mitadMitadMitadMitadAltoCasa);
    Point ventanaDerSupDer = make_tuple(puntoCentralPisoCasaX + (mitadMitadAnchoCasa + mitadMitadMitadAnchoCasa),
        puntoCentralPisoCasaY + mitadAltoCasa - mitadMitadMitadMitadAltoCasa);
    Point ventanaDerInfDer = make_tuple(puntoCentralPisoCasaX + (mitadMitadAnchoCasa + mitadMitadMitadAnchoCasa),
        puntoCentralPisoCasaY + mitadMitadAltoCasa + mitadMitadMitadMitadAltoCasa);
    Point ventanaDerInfIzq = make_tuple(puntoCentralPisoCasaX + mitadMitadMitadAnchoCasa,
        puntoCentralPisoCasaY + mitadMitadAltoCasa + mitadMitadMitadMitadAltoCasa);

    drawLine(ventanaDerSupDer,ventanaDerSupIzq);
    drawLine(ventanaDerSupIzq,ventanaDerInfIzq);
    drawLine(ventanaDerInfIzq,ventanaDerInfDer);
    drawLine(ventanaDerInfDer,ventanaDerSupDer);



    glFlush();

    //glColor3f(1.0,0.0,0.0);
    
}

int main(int argc, char **argv)
{
    anchoPantalla = 400;
    altoPantalla = 300;
    anchoCasa = 50;
    altoCasa = 100;
    puntoCentralPisoCasa = make_tuple(anchoPantalla / 2, 10);   
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(anchoPantalla, altoPantalla);
    glutCreateWindow("Ejemplo");


    init();
    glutDisplayFunc(display);
    glutMainLoop();
}