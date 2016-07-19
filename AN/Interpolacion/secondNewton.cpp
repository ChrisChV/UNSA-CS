#include <iostream>
#include <fstream>
#include <algorithm>
#include "funcion.h"

using namespace std;

int main(){
	ofstream archivo("granew.m");
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	auto puntos = getPuntos();
	string x = "x = [";
	string y = "y = [";
	cout<<"A"<<endl;
	for(auto iter = puntos.begin(); iter != puntos.end(); ++iter){
		x += to_string((*iter).x) + ",";
		y += to_string((*iter).y) + ",";
	}
	x.pop_back();
	y.pop_back();
	x += "];";
	y += "];";
	archivo<<x<<endl;
	archivo<<y<<endl;
	archivo<<"plot(x,y,'*')"<<endl;
	string x1 = "x = [";
	string y1 = "y = [";
	cout<<"A"<<endl;
	for(float i = -1; i <= 6; i+= 0.01){
		x1 += to_string(i) + ",";
		y1 += to_string(fun(i)) + ",";
	}
	x1.pop_back();
	y1.pop_back();
	x1 += "];";
	y1 += "];";
	archivo<<x1<<endl;
	archivo<<y1<<endl;
	cout<<"A"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x,y,'r')"<<endl;
	archivo<<"pause()"<<endl;
	archivo.close();
	system("chmod 755 granew.m");
	system("./granew.m");
}