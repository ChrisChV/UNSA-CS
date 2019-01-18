#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "OperacionesMatriz.h"

void llenarRandom(Matriz & O){
	srand(time(NULL));
	for(int i = 0; i < O[0].size(); i++){
		O[0][i] = rand() %  10 + 1;
	}
}

float calcularError(Matriz O, Matriz X, M_Columna Y, Matriz & h){
	h = O * X;
	float sum = 0;
	for(int i = 0; i < h[0].size(); i++){
		sum += pow(Y[i] - h[0][i],2);
	}
	return sum / (2 * h[0].size());
}

float derivada(Matriz h, Matriz X, M_Columna Y, int indexA){
	float sum = 0;
	for(int i = 0; i < h[0].size(); i++){
		if(indexA == 0) sum += (Y[i] - h[0][i]) * -1;
		else{
			sum += (Y[i] - h[0][i]) * (X[indexA][i] * -1);
		}
	}
	return sum / h[0].size();
}

void generarGrafica(string fi, Matriz X, M_Columna Y, Matriz O){
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x = [";
	for(int i = 0; i < X[0].size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(X[1][i]);
	}
	archivo<<"];"<<endl;
	archivo<<"y = [";
	for(int i = 0; i < Y.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(Y[i]);
	}
	archivo<<"];"<<endl;
	archivo<<"plot(x,y,'*')"<<endl;
	archivo<<"fun = @(x) "<<O[0][0]<<"+ x *"<<O[0][1]<<endl;
	archivo<<"tempX = linspace(0,5,100);"<<endl;
	archivo<<"tempY = fun(tempX);"<<endl;
	archivo<<"hold on;"<<endl;
	archivo<<"plot(tempX,tempY)"<<endl;
	archivo<<"pause();"<<endl;
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
}

void regresion(Matriz X, M_Columna Y, float umbral){
	Matriz O;
	O.push_back(vector<Num>(X.size()));
	llenarRandom(O);
	O = {{4,7}};
	Matriz h;
	float error = calcularError(O,X,Y,h);
	cout<<error<<endl;
	generarGrafica("test",X,Y,O);
	while(error > umbral){
		Matriz temp = O;
		temp[0][0] = O[0][0] - 0.1 *  derivada(h,X,Y,0);
		for(int i = 1; i < O[0].size(); i++){			
			temp[0][i] = O[0][i] - 0.1 * derivada(h,X,Y,i);
		}
		O = temp;
		error = calcularError(O,X,Y,h);
	}
	generarGrafica("test",X,Y,O);
}
