#ifndef VENTANA_H
#define VENTANA_H
#include <graphics.h>
#include <cmath>

class Ventana
{
    private:
        int Xd,Yd,X,Y,X0,Y0;
        float Fx,Fy;
    public:
        Ventana(){
        	Xd = 0;
        	Yd = 0;
        	X = 0;
        	Y = 0;
        	X0 = 0;
        	Y0 = 0;
        	Fx = 0;
        	Fy = 0;
        };
        void dimencion(int x,int y){
        	Xd=x;
    		Yd=y;
        };
        void origencoord(int x0,int y0){
        	X0=x0;
    		Y0=y0;
        };
        void factor(float a, float b){
        	Fx=a;
    		Fy=b;
        };
        void posicion(float x,float y, int color){
        	X=X0+Fx*x;
    		Y=Y0-Fy*y;

    		putpixel(X,Y,color);
        };
};

#endif // VENTANA_H
