#include <iostream>
#include "CImg.h"
#include <thread>
#include <mutex>
#include "Bloque.h"
#include <fstream>
#include <vector>

using namespace std;

using namespace cimg_library;

typedef CImg<float> Imagen;

mutex m;

void pintar(Imagen *img, Coordenadas x0, Coordenadas y0, Coordenadas xf,Coordenadas yf, float color){
	for(int i = x0; i <= xf; i++){
		for(int j = y0; j <= yf; j++){
			if(i == 0 or j == 0 or i ==xf or j == yf){
				(*img)(i,j,0) = color;	
			}
			(*img)(i,j,0) = color;
		}
	}
}

Imagen descomprimir(ifstream archivo){
	int i;
	int xf;
	int yf;
	archivo.read(reinterpret_cast<char*>(&i),sizeof(int));
	archivo.read(reinterpret_cast<char*>(&xf),sizeof(int));
	archivo.read(reinterpret_cast<char*>(&yf),sizeof(int));
	vector<thread> tt;
	Imagen * img = new Imagen(xf + 1,yf + 1,1,3,0);
	for(int j = 0; j < i; j++){
		Bloque b;
		archivo.read(reinterpret_cast<char*>(&b),sizeof(Bloque));
		tt.push_back(thread(pintar,img,b.x0,b.y0,b.xf,b.yf,b.color));
	}
	for(auto iter = tt.begin(); iter != tt.end(); ++iter){
		(*iter).join();
	}
}

bool todosSonIguales(Imagen &img, Coordenadas x0, Coordenadas y0, Coordenadas xf,
		Coordenadas yf, float &color){
	float coloractual = -1;
	for(Coordenadas i = x0; i <= xf; i++){
		for(Coordenadas j = y0; j <= yf; j++){
			if(coloractual == -1) coloractual = img(i,j,0);
			else{
				float temp = img(i,j,0);
				if(coloractual - PRESICION >= temp or coloractual + PRESICION <= temp){
					return false;
				}
			}
		}
	}
	return true;
}

void escribirArchivo(Coordenadas x0, Coordenadas y0, Coordenadas xf, Coordenadas yf, float color, ofstream * archivo){
	Bloque b(x0,y0,xf,yf,color);
	archivo->write(reinterpret_cast<char*>(&b),sizeof(Bloque));
}

void QuadTree(Imagen &img, Coordenadas x0, Coordenadas y0, Coordenadas xf, Coordenadas yf, ofstream * archivo, int * i){
	float color;
	/*cout<<"x0->"<<x0<<endl;
	cout<<"y0->"<<y0<<endl;
	cout<<"xf->"<<xf<<endl;
	cout<<"yf->"<<yf<<endl;
	*/
	int temp1 = ((xf - x0) / 2);
	int temp2 = ((yf - y0) / 2);
	if((x0 == xf and y0 == yf) or (temp1 == 0 and temp2 == 0)){
		m.lock();
			*i = *i + 1;
			escribirArchivo(x0,y0,xf,yf,img(x0,y0,0),archivo);
		m.unlock();
		return;
	}
	if(todosSonIguales(img,x0,y0,xf,yf,color)){
		m.lock();
			*i = *i + 1;
			escribirArchivo(x0,y0,xf,yf,color,archivo);
		m.unlock();
		return;
	}
	Coordenadas xM = temp1 + x0;
	Coordenadas yM = temp2 + y0;
	/*cout<<"1->"<<xf-x0<<endl;
	cout<<"2->"<<(xf-x0) / 2<<endl;
	cout<<"Xm->"<<xM<<endl;
	cout<<"Ym->"<<yM<<endl;
	*/
	thread superiorIzquierda(QuadTree,ref(img),x0,y0,xM,yM,archivo,i);
	superiorIzquierda.join();
	thread superiorDerecha(QuadTree,ref(img),xM,y0,xf,yM,archivo,i);
	superiorDerecha.join();
	thread inferiorIzquierda(QuadTree,ref(img),x0,yM,xM,yf,archivo,i);
	inferiorIzquierda.join();
	thread inferiorDerecha(QuadTree,ref(img),xM,yM,xf,yf,archivo,i);
	inferiorDerecha.join();
	
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
		}
	}
	return I;
}

int main(){
	bool f;
	cout<<"comprimir(0) - descomprimir(1)->";
	cin>>f;
	if(!f){
		Imagen I("P.jpg");
		Imagen gris =  escalaGris(I);
		ofstream * archivo = new ofstream("ejem.dat", ios::binary);
		int *i = new int(0);
		int xf = gris.width() - 1;
		int yf = gris.height() - 1;
		archivo->write(reinterpret_cast<char*>(i),sizeof(int));
		archivo->write(reinterpret_cast<char*>(&xf),sizeof(int));
		archivo->write(reinterpret_cast<char*>(&yf),sizeof(int));
		QuadTree(gris,0,0,gris.width() - 1, gris.height() - 1,archivo,i);
		archivo->seekp(0,ios::beg);
		archivo->write(reinterpret_cast<char*>(i),sizeof(int));
		cout<<"I->"<<*i<<endl;
	}
	else{
		Imagen I("P.jpg");
		Imagen gris = escalaGris(I);
		gris.display();
	}
}


