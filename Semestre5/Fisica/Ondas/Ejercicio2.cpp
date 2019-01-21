#include <iostream>
#include "Ventana.h"
#include <cmath>

using namespace std;

float Y1(float x){
    if(x >= 0) return 0;
    if(x <= -5 * M_PI) return 0;
    return -2 *sin(x) -5 * M_PI;
}

float Y2(float x){
    return 3 * pow(M_E,-1 * pow(x,2));
}

int main()
{
    int gd = DETECT,gm;
    initgraph(&gd,&gm,NULL);
    int k=3;
    int w=1;
    Ventana v;
    Ventana v2;
    Ventana v3;
    v.dimencion(500,300);
    v.origencoord(0,100);
    v.factor(64,10);
    v2.dimencion(500,300);
    v2.origencoord(0,270);
    v2.factor(64,10);
    for(float t=0;t<20;t+=0.1)
    {
        for(float x=0; x<=10;x+=0.01)
        {
            float y1 = Y1(k * x - w * t);
            float y2 = Y2(k * (x-5) + w * t);
            float y=y1+y2;
            if(y != y2) v.posicion(x,y,3);
            if(y1 != 0)v.posicion(x,y1,7);
            v2.posicion(x,y2,13);
        }
        delay(20);
        cleardevice();
    }
    closegraph();
}