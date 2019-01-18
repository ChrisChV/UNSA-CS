#include <GL/glut.h>


void init(void){
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,250.0,0.0,150.0);
}

void line1(void){
    glBegin(GL_LINES);
        glVertex2i(10,145);
        glVertex2i(180,15);
    glEnd();
}

void line2(void){
    glBegin(GL_LINES);
        glVertex2i(60,145);
        glVertex2i(230,15);
    glEnd();
}

void lineSegment(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0,0.4,0.2);
    line1();
    glFlush();


    glLineWidth(3);
    glColor3f(1.0,0.0,0.0);
    line2();
    glFlush();

    glLineWidth(1);
    glColor3f(0.0,0.4,0.2);
    line1();
    glClearColor(0.8,0.8,0.8,0.0);
    glFlush();

    //glColor3f(1.0,0.0,0.0);
    
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(400,300);
    glutCreateWindow("Ejemplo");
    init();
    glutDisplayFunc(lineSegment);
    glutMainLoop();
}