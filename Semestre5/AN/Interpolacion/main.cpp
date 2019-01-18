#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

typedef long double Num;

void generarPolinomio(vector<Num> X, vector<Num> Y){
	ofstream archivo("funcion.h");
	archivo<<"#ifndef FUNCION_H"<<endl;
	archivo<<"#define FUNCION_H"<<endl;
	archivo<<"#include <iostream>"<<endl;
	archivo<<"#include <cmath>"<<endl;
	archivo<<"typedef long double Num;"<<endl<<endl;
	archivo<<"Num fun(Num x){"<<endl;
	string L = "";
	for(int i = 0; i < Y.size(); i++){
		L = L + to_string(Y[i]);
		L = L + "*((";
		for(int j = 0; j < X.size(); j++){
			if(i != j){
				L = L + "(x-" + to_string(X[j]) + ")*";
			}
		}
		L.pop_back();
		L = L + ")/(";
		for(int j = 0; j < X.size(); j++){
			if(i != j){
				L = L + "(" +  to_string(X[i]) + "-" + to_string(X[j]) + ")*";
			}
		}
		L.pop_back();
		L = L + "))+";
	}
	L.pop_back();
	archivo<<"return "<<L<<";"<<endl;
	archivo<<"}"<<endl<<endl;
	archivo<<"#endif";
	archivo.close();
	system("g++ -std=c++11 second.cpp -o run2");
	system("./run2");
}

int main(){
	vector<Num> X = {0,1,1.5,2.4,3,4};
	vector<Num> Y = {cos(0),cos(1),cos(1.5),cos(2.4),cos(3),cos(4)};
	generarPolinomio(X,Y);
}