#include <iostream>
#include <cmath>
#include "OperacionesMatriz.h"

using namespace std;

Num buscarMayor(Lista & b){
	Num res = -1;
	for(auto iter = b.begin(); iter != b.end(); iter++){
		if(abs(*iter) > res or res == -1) res = abs(*iter);
	}
	return res;
}

void MetodoPotencias(int iteraciones, Matriz & m, Lista ini){
	Lista u;
	Lista v = ini;
	for(int i = 0; i < iteraciones; i++){
		u = m * v;
		Num dominante = buscarMayor(u);
		v = u * (1.0 / dominante);
		cout<<"------U_"<<i+1<<"------"<<endl;
		mostrarLista(u);
		cout<<"------V_"<<i+1<<"------"<<endl;
		mostrarLista(v);
	}
}

int main(int argc, char * argv[]){
	if(argc != 2){
		cout<<"Faltan argumentos: <numIteraciones>"<<endl;
		return 0;
	}
	string temp(argv[1]);
	int it = stoi(temp);
	Matriz m = {{3,-1,0},{-1,2,-1},{0,-1,3}};
	Lista ini = {1,1,1};
	MetodoPotencias(it,m,ini);
}