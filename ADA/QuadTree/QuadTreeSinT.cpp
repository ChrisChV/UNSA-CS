#include <iostream>
#include "CImg.h"
#include "Bloque.h"
#include <fstream>
#include <vector>

using namespace std;

using namespace cimg_library;

typedef CImg<float> Imagen;

int PRESICION;

int PRESICION2;

void pintar(Imagen &img, Coordenadas x0, Coordenadas y0, Coordenadas xf,Coordenadas yf, Color color){
	for(int i = x0; i <= xf; i++){
		for(int j = y0; j <= yf; j++){
			if(i == 0 or j == 0 or i ==xf or j == yf){
				img(i,j,0) = 50;	
				img(i,j,1) = 150;	
				img(i,j,2) = 250;	
			}
			else{
				img(i,j,0) = color;
				img(i,j,1) = color;
				img(i,j,2) = color;
			}
		}
	}
}

Imagen descomprimir(ifstream &archivo){
	int i;
	Coordenadas xf;
	Coordenadas yf;
	archivo.read(reinterpret_cast<char*>(&i),sizeof(int));
	archivo.read(reinterpret_cast<char*>(&xf),sizeof(Coordenadas));
	archivo.read(reinterpret_cast<char*>(&yf),sizeof(Coordenadas));
	Imagen img(xf + 1,yf + 1,1,3,0);
	cout<<"i->"<<i<<endl;
	for(int j = 0; j < i; j++){
		Bloque b;
		archivo.read(reinterpret_cast<char*>(&b),sizeof(Bloque));
		pintar(img,b.x0,b.y0,b.xf,b.yf,b.color);
	}
	return img;
}

bool todosSonIguales(Imagen &img, Coordenadas x0, Coordenadas y0, Coordenadas xf,
		Coordenadas yf, Color &coloractual){
	coloractual = -1;
	for(Coordenadas i = x0; i <= xf; i++){
		for(Coordenadas j = y0; j <= yf; j++){
			if(coloractual == -1) coloractual = img(i,j,0);
			else{
				Color temp = img(i,j,0);
				if(coloractual - PRESICION >= temp or coloractual + PRESICION <= temp){
					return false;
				}
			}
		}
	}
	return true;
}

void escribirArchivo(Coordenadas x0, Coordenadas y0, Coordenadas xf, Coordenadas yf, Color color, ofstream & archivo){
	Bloque b(x0,y0,xf,yf,color);
	archivo.write(reinterpret_cast<char*>(&b),sizeof(Bloque));
}

void QuadTree(Imagen &img, Coordenadas x0, Coordenadas y0, Coordenadas xf, Coordenadas yf, ofstream & archivo, int & i){
	Color color;
	cout<<"x0->"<<x0<<endl;
	cout<<"y0->"<<y0<<endl;
	cout<<"xf->"<<xf<<endl;
	cout<<"yf->"<<yf<<endl;
	Coordenadas temp1 = ((xf - x0) / 2);
	Coordenadas temp2 = ((yf - y0) / 2);
	if((x0 == xf and y0 == yf) or (temp1 == 0 and temp2 == 0) or (xf - x0) * (yf -y0) <= PRESICION2){
		i++;
		Color color = img(x0,y0,0);
		escribirArchivo(x0,y0,xf,yf,color,archivo);
		cout<<"a"<<endl;
		return;
	}
	if(todosSonIguales(img,x0,y0,xf,yf,color)){
		i++;
		escribirArchivo(x0,y0,xf,yf,color,archivo);
		cout<<"b"<<endl;
		return;
	}
	Coordenadas xM = temp1 + x0;
	Coordenadas yM = temp2 + y0;
	
	cout<<"Xm->"<<xM<<endl;
	cout<<"Ym->"<<yM<<endl;
	
	QuadTree(img,x0,y0,xM,yM,archivo,i);
	QuadTree(img,xM + 1,y0,xf,yM,archivo,i);
	QuadTree(img,x0,yM + 1,xM,yf,archivo,i);
	QuadTree(img,xM + 1,yM + 1,xf,yf,archivo,i);
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
	string file;	
	cout<<"comprimir(0) - descomprimir(1)->";
	cin>>f;
	if(!f){
		cout<<"Ingrese la imagen->";
		cin>>file;
		cout<<"Ingrese la presicion del color->";
		cin>>PRESICION;
		cout<<"Ingrese la Presicion de los cuadros->";
		cin>>PRESICION2;
		Imagen I(file.c_str());
		Imagen gris =  escalaGris(I);
		ofstream archivo("ejem2.dat", ios::binary);
		int i = 0;
		Coordenadas xf = gris.width() - 1;
		Coordenadas yf = gris.height() - 1;
		cout<<"a"<<endl;
		archivo.write(reinterpret_cast<char*>(&i),sizeof(int));
		archivo.write(reinterpret_cast<char*>(&xf),sizeof(Coordenadas));
		archivo.write(reinterpret_cast<char*>(&yf),sizeof(Coordenadas));
		QuadTree(gris,0,0,gris.width() - 1, gris.height() - 1,archivo,i);
		archivo.seekp(0,ios::beg);
		cout<<"I->"<<i<<endl;
		int a = i;
		archivo.write(reinterpret_cast<char*>(&a),sizeof(int));
		archivo.close();
	}
	else{
		ifstream archivo("ejem2.dat", ios::binary);
		Imagen I(descomprimir(archivo));
		I.display();
		archivo.close();
	}
}


