#include <iostream>
#include "Ventana.h"
#include <cmath>

using namespace std;

int main()
{
    int gd = DETECT,gm;
    initgraph(&gd,&gm,NULL);
    int k=3;
    int w=1;
    Ventana v;
    v.dimencion(500,300);
    v.origencoord(0,250);
    v.factor(64,50);
    for(float t=0;t<20;t+=0.1)
    {
        for(float x=0; x<=10;x+=0.01)
        {
            float y1=2*sin(k*x-w*t);
            float y2=2*sin(k*x+w*t);
            float y=y1+y2;
            v.posicion(x,y,3);
            v.posicion(x,y1,7);
            v.posicion(x,y2,13);
        }
        //delay(50);
        cleardevice();

    }
    closegraph();
}