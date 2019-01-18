#ifndef BLOQUE_H
#define BLOQUE_H

#include <iostream>

using namespace std;

typedef int Coordenadas;

typedef int Color;

class Bloque{
	public:
		Coordenadas x0;
		Coordenadas y0;
		Coordenadas xf;
		Coordenadas yf;
		Bloque(){
			x0 = 0;
			y0 = 0;
			xf = 0;
			yf = 0;
		}
		Bloque(Coordenadas x0, Coordenadas y0, Coordenadas xf, Coordenadas yf){
			this->x0 = x0;
			this->y0 = y0;
			this->xf = xf;
			this->yf = yf;
		}
};

#endif