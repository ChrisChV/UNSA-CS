#include <iostream>
#include <fstream>
#include <vector>
#include "funcion.h"

using namespace std;

int main(){
	vector<Num> X = {0,1,1.5,2.4,3,4};
	vector<Num> Y = {cos(0),cos(1),cos(1.5),cos(2.4),cos(3),cos(4)};
	ofstream archivo("gra.m");
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	string x = "x1 = [";
	for(int i = 0; i < X.size(); i++){
		x = x + to_string(X[i]) + ",";
	}
	x.pop_back();
	x = x + "];";
	archivo<<x<<endl;
	string y = "y1 =[";
	for(int i = 0; i < Y.size(); i++){
		y = y + to_string(Y[i]) + ",";
	}
	y.pop_back();
	y = y + "];";
	archivo<<y<<endl;
	archivo<<"plot(x1,y1,'r*')"<<endl;
	archivo<<"grid on"<<endl;
	archivo<<"hold on"<<endl;
	x = "x2 = [";
	y = "y2 = [";
	string x1 = "x3 = [";
	string y1 = "y3 = [";
	for(float i = -1; i < 6; i = i + 0.01){
		x1 = x1 + to_string(i) + ",";
		y1 = y1 + to_string(cos(i)) + ",";
		x = x + to_string(i) + ",";
		y = y + to_string(fun(i)) + ",";
	}
	x.pop_back();
	x = x + "];";
	y.pop_back();
	y = y + "];";
	x1.pop_back();
	x1 = x1 + "];";
	y1.pop_back();
	y1 = y1 + "];";
	archivo<<x<<endl;
	archivo<<y<<endl;
	archivo<<x1<<endl;
	archivo<<y1<<endl;
	archivo<<"plot(x2,y2)"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x3,y3,'g')"<<endl;
	archivo<<"pause()";
	archivo.close();
	system("chmod 755 gra.m");
	system("./gra.m");
}