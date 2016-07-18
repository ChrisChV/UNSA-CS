#ifndef PUNTO_H
#define PUNTO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <map>
#include <tuple>
#include <time.h>

using namespace std;

typedef float Coordenada;

class Punto{
public:
	Coordenada x;
	Coordenada y;
	float angulo;
	float distancia;
	Punto(){x = 0; y = 0; angulo = 0; distancia = 0;}; 
	Punto(Coordenada x, Coordenada y){this->x = x; this->y = y; angulo = 0; distancia = 0;};
	void HallarAngulo(Punto * p);
	void imprimirPunto();
	bool operator ==(Punto p){
		if(this->x == p.x and this->y == p.y) return true;
		return false;
	};
	bool operator < (Punto u){
			if(this->x < u.x) return true;
			else if(this->x > u.x) return false;
			else{
				if(this->y <= u.y) return true;
				else return false;
			}
	}
};

void Punto::imprimirPunto(){
	cout<<"("<<x<<","<<y<<")";
}

void Punto::HallarAngulo(Punto * p){
	float xi = p->x - this->x;
	float yi = p->y - this->y;
	if(xi == 0){
		angulo = -10000;
	}
	else{
		float m = yi/xi;
		angulo = atan(m);	
	}
	
}

float Distancia(Punto * a, Punto * b){
	return sqrt(pow(b->x-a->x,2) + pow(b->y-a->x,2));
}

bool sortByX(Punto * a, Punto * b){
	return (a->x < b->x);
}

bool sortByY(Punto * a, Punto * b){
	return (a->y < b->y);
}

bool _sortByY(Punto * a, Punto * b){
	return (a->y > b->y);
}

bool sortByAngulo(Punto * a, Punto * b){
	/*if(a.angulo == b.angulo){
		return (a.distancia < b.distancia);
	}*/
	return (a->angulo < b->angulo);
}

bool _sortByAngulo(Punto * a, Punto * b){
	return (a->angulo > b->angulo);
}

void HallarAngulos(vector<Punto *> &puntos, Punto* p){
	for(auto iter = puntos.begin(); iter != puntos.end(); ++iter){
		(*iter)->distancia = Distancia(p,*iter);
		(*iter)->HallarAngulo(p);
	}
}

int productoVectorial(Punto * a, Punto * b, Punto * c){
	return (b->x - a->x) * (c->y - a->y) - (b->y - a->y)*(c->x - a->x);
}


void generarGrafica(string fi, vector<Punto*> &all, vector<Punto*> &convex){
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i]->x);
	}
	archivo<<"];"<<endl;
	archivo<<"y = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i]->y);
	}
	archivo<<"];"<<endl;
	archivo<<"plot(x,y,'*')"<<endl;
	auto iter1 = convex.begin();
	auto iter2 = iter1 + 1;
	for(; iter2 != convex.end(); ++iter1, ++iter2){
		archivo<<"x = ["<<(*iter1)->x<<","<<(*iter2)->x<<"];"<<endl;
		archivo<<"y = ["<<(*iter1)->y<<","<<(*iter2)->y<<"];"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x,y,'r')"<<endl;
	}
	archivo<<"x = ["<<convex.back()->x<<","<<convex.front()->x<<"];"<<endl;
	archivo<<"y = ["<<convex.back()->y<<","<<convex.front()->y<<"];"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x,y,'r')"<<endl;
	archivo<<"pause()";
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
}

void generarGrafica(string fi, vector<Punto*> all){
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x1 = [";
	for(auto iter = all.begin(); iter != all.end(); ++iter){
		if(iter != all.begin()) archivo<<",";
		archivo<<(*iter)->x;
	}
	archivo<<"];"<<endl;
	archivo<<"y1 = [";
	for(auto iter = all.begin(); iter != all.end(); ++iter){
		if(iter != all.begin())archivo<<",";
		archivo<<(*iter)->y;
	} 
	archivo<<"];"<<endl;
	archivo<<"grid on"<<endl;
	archivo<<"plot(x1,y1,'*')"<<endl;
	archivo<<"pause()"<<endl;
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
	//system("octave-cli");
	//string com = "run " + file;
	//system(com.c_str());
	//system("graf()");
}

vector<Punto*> generarPuntosAleatorios(int xMax, int yMax, int numPuntos){
	vector<Punto*> res;
	map<tuple<int,int>,bool> m;
	map<int,bool> xs;
	for(int i = 0; i < numPuntos; i++){
		int x = rand() % xMax + 1;
		int y = rand() % yMax + 1;
		auto tt = make_tuple(x,y);
		auto iter = m.find(tt);
		auto iter2 = xs.find(x);
		if(iter == m.end() and iter2 == xs.end()){
			m[tt] = true;
			xs[x] = true;
			res.push_back(new Punto(x,y));	
		}
		else{
			i--;
		}
	}
	return res;
}

#endif