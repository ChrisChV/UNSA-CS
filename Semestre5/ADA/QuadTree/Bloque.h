#ifndef BLOQUE_H
#define BLOQUE_H

#include <iostream>

using namespace std;

typedef short Coordenadas;

typedef short Color;

class Bloque{
	public:
		Coordenadas x0;
		Coordenadas y0;
		Coordenadas xf;
		Coordenadas yf;
		Color color;
		Bloque(){
			x0 = 0;
			y0 = 0;
			xf = 0;
			yf = 0;
			color = 0;
		}
		Bloque(Coordenadas x0, Coordenadas y0, Coordenadas xf, Coordenadas yf, Color color){
			this->x0 = x0;
			this->y0 = y0;
			this->xf = xf;
			this->yf = yf;
			this->color = color;
		}
};

#endif