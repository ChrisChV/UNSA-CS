#include <iostream>
#include "CImg.h"
#include "Bloque.h"
#include <fstream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>

using namespace std;

using namespace cimg_library;

typedef CImg<float> Imagen;

typedef double Medida;

void pintar(Imagen &img, Bloque & bloque, Coordenadas medioX, Coordenadas medioY){
	for(int i = bloque.x0; i < bloque.xf; i++){
		img(i,medioY,0) = 50;	
		img(i,medioY,1) = 150;
		//img(i,medioY,2) = 250;
	}
	for(int i = bloque.y0; i < bloque.yf; i++){
		img(medioX,i,0) = 50;	
		img(medioX,i,1) = 150;
		//img(medioX,i,2) = 250;	
	}
		
}

void printBLoque(Imagen &img, Bloque &bloque){
	int x0 = bloque.x0;
	int xf = bloque.xf;
	int y0 = bloque.y0;
	int yf = bloque.yf;
	for(int i = bloque.x0; i < bloque.xf; i++){
		for(int j = bloque.y0; j < bloque.yf; j++){
			cout<<img(i,j,2)<<endl;
		}
	}
}

Medida media(Imagen &img, Bloque & bloque){
	Medida sum = 0;
	for(int i = bloque.x0; i < bloque.xf; i++){
		for(int j = bloque.y0; j < bloque.yf; j++){
			sum += img(i,j,2);
		}
	}
	return sum / ((bloque.xf - bloque.x0) * (bloque.yf - bloque.y0));
}

Medida varianza(Imagen &img, Bloque & bloque){
	Medida m = media(img, bloque);
	Medida sum = 0;
	for(int i = bloque.x0; i < bloque.xf; i++){
		for(int j = bloque.y0; j < bloque.yf; j++){
			sum += pow(img(i,j,2) - m,2);
		}
	}
	return sum / ((bloque.xf - bloque.x0) * (bloque.yf - bloque.y0));	
}



void _QuadTree(Imagen &img, Bloque & bloque, Medida & limite){
	int x0 = bloque.x0;
	int xf = bloque.xf;
	int y0 = bloque.y0;
	int yf = bloque.yf;
	Coordenadas medioX = (xf + x0) / 2;
	Coordenadas medioY = (yf + y0) / 2;	
	long tam = (xf - x0) * (yf - y0);
	if(tam == 0 or tam == 1){
		return;
	}
//	cout<<"TAM->"<<tam<<endl;
//	printBLoque(img,bloque);
	Medida var = varianza(img,bloque);
//	cout<<"VAR->"<<var<<endl;
	if(var > limite){ 
		Bloque norOeste(x0,y0,medioX,medioY);
		Bloque norEste(medioX,y0,xf,medioY);
		Bloque surOeste(x0,medioY,medioX,yf);
		Bloque surEste(medioX,medioY,xf,yf);
		pintar(img,bloque,medioX,medioY);
		_QuadTree(img,norOeste,limite);
		_QuadTree(img,norEste,limite);
		_QuadTree(img,surOeste,limite);
		_QuadTree(img,surEste,limite);
	}

}

Imagen escalaGris(Imagen &S){
	Imagen I(S.width(),S.height(),1,3,0);
	for(Coordenadas i = 0; i < S.width(); i++){
		for(Coordenadas j = 0; j < S.height(); j++){
			float R = S(i,j,0);
			float G = S(i,j,1);
			float B = S(i,j,2);
			float promedio = R * 0.3 + G * 0.59 + B * 0.11;
			I(i,j,0) = promedio;
			I(i,j,1) = promedio;
			I(i,j,2) = promedio;
		}
	}
	return I;
}

int main(int argc , char * argv[]){
	if(argc != 3){
		cout<<"Faltan argumentos <img> <presicion>"<<endl;
		return 0;
	}
	string file(argv[1]);
	string temp(argv[2]);
	Medida limite = stod(temp);
	Imagen I(file.c_str());
	Imagen gris = escalaGris(I);
	Bloque b = Bloque(0,0,gris.width()-1,gris.height()-1);
	_QuadTree(gris,b,limite);
	gris.display();

}


