#include <GL/glut.h>
#include <iostream>
#include <tuple>

using namespace std;

typedef tuple<float,float> Point;

float anchoPantalla;
float altoPantalla;

typedef struct{
    float R;
    float G;
    float B;
}Color;

class Casa{
public:
    Casa(){};
    Casa(float ancho, float alto, float centralX, float centralY){
        puntoCentralPisoCasa = make_tuple(centralX,centralY);
        anchoCasa = ancho;
        altoCasa = alto;
        colorCasa.R = 1.0; colorCasa.G = 0.0; colorCasa.B = 0.0;
        colorTecho.R = 0.0; colorTecho.G = 0.4; colorTecho.B = 0.2;
        colorPuerta.R = 0.5; colorPuerta.G = 0.35; colorPuerta.B = 0.05;
        colorVentanas.R = 0.196078; colorVentanas.G = 0.6; colorVentanas.B = 0.8;
        grosorCasa = 1;
        grosorTecho = 1;
        grosorPuerta = 1;
        grosorVentanas = 1;
    }
    void drawCasa();
    Point puntoCentralPisoCasa;
    float anchoCasa;
    float altoCasa;
    Color colorCasa;
    Color colorTecho;
    Color colorPuerta;
    Color colorVentanas;
    float grosorCasa;
    float grosorTecho;
    float grosorPuerta;
    float grosorVentanas;
    
};


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

void Casa::drawCasa(){
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
    glColor3f(colorCasa.R,colorCasa.G,colorCasa.B);
    drawLine(paredSupIzq,paredSupDer);
    drawLine(paredSupDer,paredInfDer);
    drawLine(paredInfDer,paredInfIzq);
    drawLine(paredInfIzq,paredSupIzq);
    glFlush();

    glLineWidth(grosorTecho);
    glColor3f(colorTecho.R,colorTecho.G,colorTecho.B);
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

    glColor3f(colorPuerta.R,colorPuerta.G,colorPuerta.B);
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

    glColor3f(colorVentanas.R,colorVentanas.G,colorVentanas.B);
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
}


Casa casita;

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    casita.drawCasa();
    
    glFlush();

    
    
}


int main(int argc, char **argv)
{
    anchoPantalla = 400;
    altoPantalla = 300;
    int anchoCasa = 25;
    int altoCasa = 50;
    casita = Casa(anchoCasa, altoCasa, 200, 20);
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(anchoPantalla, altoPantalla);
    glutCreateWindow("Ejemplo");


    init();
    glutDisplayFunc(display);
    glutMainLoop();
}