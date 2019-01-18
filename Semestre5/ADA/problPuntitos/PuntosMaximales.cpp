#include <iostream>
#include "Punto.h"
#include <fstream>
#include <algorithm>


using namespace std;

Punto hallarMayorY(vector<Punto> vec){
	if(vec.size() == 0) return Punto(-9999,-9999);
	int mayor = vec.front().y;
	Punto p = vec.front();
	for(auto iter = vec.begin() + 1; iter != vec.end(); ++iter) {
		if(mayor < (*iter).y){
			mayor = (*iter).y;
			p = *iter;
		}
	}
	return p;
}

vector<Punto> unir(vector<Punto> izquierda, vector<Punto> derecha){
	Punto mayor = hallarMayorY(derecha);
	if(mayor == Punto(-9999,-9999)) return izquierda;
	vector<Punto> res = derecha;
	for(auto iter = izquierda.begin(); iter != izquierda.end(); ++iter) if(mayor.y < (*iter).y) res.push_back(*iter);
	return res;
}

vector<Punto> PuntosMaximales(vector<Punto> vec, int ini, int fin){
	vector<Punto> res;
	if(ini == fin){
		res.push_back(vec[ini]);
		return res;
	}
	if(fin - ini == 1){
		if(vec[ini].y >= vec[fin].y){
			res.push_back(vec[ini]);
			res.push_back(vec[fin]);
			return res;
		}
		else{
			res.push_back(vec[fin]);
			return res;
		}
	}
	int M = (fin + ini) / 2;
	return unir(PuntosMaximales(vec,ini,M),PuntosMaximales(vec,M+1,fin));
}


int main(){
	srand (time(NULL));
	int xMax;
	int yMax;
	int numPuntos;
	string file;
	cout<<"Ingresa su xMax->";
	cin>>xMax;
	cout<<"Ingrese su yMax->";
	cin>>yMax;
	cout<<"Ingrese su numero maximo de puntos->";
	cin>>numPuntos;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	vector<Punto> vec = generarPuntosAleatorios(xMax,yMax,numPuntos);
	sort(vec.begin(),vec.end(),sortByX);
	//for(auto iter = vec.begin(); iter != vec.end(); ++iter) cout<<(*iter).x<<endl;
	vector<Punto> res = PuntosMaximales(vec,0,vec.size() - 1);
	generarGrafica(file,vec,res);
}




