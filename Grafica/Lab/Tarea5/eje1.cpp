#include <GL/glut.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 600, winHeight = 500;
int primitiva = -1;

enum Primitivas {POINTS = 1, LINES, STRIP, LINE_LOOP, POLYGON, TRIANGLE_STRIP, TRIANGLES, TRIANGLE_FAN, QUADS, QUAD_STRIP};

void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 500.0);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    switch(primitiva){
        case POINTS: {
            glBegin(GL_POINTS);
                glVertex2i(250,250);
                glVertex2i(100,100);
                glVertex2i(50,50);
                glVertex2i(150,100);
            glEnd();
            break;
        }
        case LINES: {
            glBegin(GL_LINES);
                glVertex2i(250,250);
                glVertex2i(100,100);
                glVertex2i(50,50);
                glVertex2i(150,100);
                glVertex2i(500,400);
                glVertex2i(300,250);
            glEnd();
            break;
        }
        case STRIP: {
            glBegin(GL_LINE_STRIP);
                glVertex2i(250,250);
                glVertex2i(200,300);
                glVertex2i(150,270);
                glVertex2i(450,276);
                glVertex2i(300,300);
                glVertex2i(330,100);
                glVertex2i(170,80);
            glEnd();
            break;
        }
        case LINE_LOOP: {
            glBegin(GL_LINE_LOOP);
                glVertex2i(200, 200);
                glVertex2i(250, 50);
                glVertex2i(300, 150);
                glVertex2i(240, 170);
                glVertex2i(230, 230);
                glVertex2i(300, 230);
            glEnd();
            break;
        }
        case POLYGON: {
            glBegin(GL_POLYGON);
                glVertex2i(200, 200);
                glVertex2i(250, 250);
                glVertex2i(300, 200);
                glVertex2i(300, 150);
                glVertex2i(250, 100);
                glVertex2i(200, 150);
            glEnd();
            break;
        }
        case TRIANGLES: {
            glBegin(GL_TRIANGLES);
                glVertex2i(150,150);
                glVertex2i(200,200);
                glVertex2i(250,140);
                glVertex2i(400,400);
                glVertex2i(360,350);
                glVertex2i(500,300);
            glEnd();
            break;
        }
        case TRIANGLE_STRIP: {
            glBegin(GL_TRIANGLE_STRIP);
                glVertex2i(100,400);                
                glVertex2i(400,400);
                glVertex2i(130,300);
                glColor3f(0.0, 1.0, 0.0);
                glVertex2i(370,300);
                glVertex2i(90,250);
                glColor3f(0.0, 0.0, 1.0);
                glVertex2i(350,230);
                glVertex2i(150,150);
            glEnd();
            break;
        }
        case TRIANGLE_FAN: {
            glBegin(GL_TRIANGLE_FAN);
                glVertex2i(200,200);
                glVertex2i(220,250);
                glVertex2i(300,240);
                glColor3f(0.0, 1.0, 0.0);
                glVertex2i(330,230);
                glColor3f(0.0, 0.0, 1.0);
                glVertex2i(400,210);
                glVertex2i(350,230);
            glEnd();
            break;
        }
        case QUADS: {
            glBegin(GL_QUADS);
                glVertex2i(150,200);
                glVertex2i(100,150);
                glVertex2i(150,100);
                glVertex2i(200,150);
            glEnd();
            break;
        }
        case QUAD_STRIP: {
            glBegin(GL_QUAD_STRIP);
                glVertex2i(100,400);
                glVertex2i(400,380);
                glVertex2i(350,350);
                glVertex2i(150,350);
                
            glEnd();
        }
        default: break;
    }
    glFlush();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Programa Primitivas");
    init();
    cout<<"Seleccione primitiva a dibujar:"<<endl;
    cout<<"1->Puntos"<<endl;
    cout<<"2->Líneas"<<endl;
    cout<<"3->Conjunto de Líneas"<<endl;
    cout<<"4->Loop de Líneas"<<endl;
    cout<<"5->Poligono"<<endl;
    cout<<"6->Conjunto de Triangulos"<<endl;
    cout<<"7->Triangulos"<<endl;
    cout<<"8->Triangle Fan"<<endl;
    cout<<"9->Cuadrilateros"<<endl;
    cout<<"10->Conjunto de Cuadrilateros"<<endl;
    cout<<"Respuesta->";
    cin>>primitiva;


    glutDisplayFunc(display);


    glutMainLoop();
    return 0;
}




