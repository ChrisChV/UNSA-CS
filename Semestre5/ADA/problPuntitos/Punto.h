#ifndef PUNTO_H
#define PUNTO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

class Punto{
public:
	int x;
	int y;
	Punto(){x = 0; y = 0;}; 
	Punto(int x, int y){this->x = x; this->y = y;};
	bool operator ==(Punto p){
		if(this->x == p.x and this->y == p.x) return true;
		return false;
	};
};

bool sortByX(Punto a, Punto b){
	return (a.x < b.x);
}

bool sortByY(Punto a, Punto b){
	return (a.y < b.y);
}

void generarGrafica(string fi, vector<Punto> all, vector<Punto> maximales){
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x1 = [";
	for(auto iter = all.begin(); iter != all.end(); ++iter){
		if(iter != all.begin()) archivo<<",";
		archivo<<(*iter).x;
	}
	archivo<<"];"<<endl;
	archivo<<"y1 = [";
	for(auto iter = all.begin(); iter != all.end(); ++iter){
		if(iter != all.begin())archivo<<",";
		archivo<<(*iter).y;
	} 
	archivo<<"];"<<endl;
	archivo<<"x2 = [";
	for(auto iter = maximales.begin(); iter != maximales.end(); ++iter){
		if(iter != maximales.begin()) archivo<<",";
		archivo<<(*iter).x;
	} 
	archivo<<"];"<<endl;
	archivo<<"y2 = [";
	for(auto iter = maximales.begin(); iter != maximales.end(); ++iter){
		if(iter != maximales.begin()) archivo<<",";
		archivo<<(*iter).y;
	} 
	archivo<<"];"<<endl;
	archivo<<"grid on"<<endl;
	archivo<<"plot(x1,y1,'*')"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x2,y2,'r*')"<<endl;
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

vector<Punto> generarPuntosAleatorios(int xMax, int yMax, int numPuntos){
	vector<Punto> res;
	for(int i = 0; i < numPuntos; i++) res.push_back(Punto(rand()%xMax,rand()%yMax));
	return res;
}

#endif