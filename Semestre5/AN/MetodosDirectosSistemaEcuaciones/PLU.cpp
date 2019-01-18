#include <iostream>
#include <tuple>
#include "Sustituciones.h"
#include "OperacionesMatriz.h"

//rhanccoan@unsa.edu.pe
//Asunto: Sistemas lineales por LUP

using namespace std;

enum Tipes{M_P,M_L,M_U};

tuple<Matriz,Matriz,Matriz> PLU(Matriz A){
	Matriz P = identidad(A.size());
	Matriz L = zeros(A.size());
	Matriz U = A;
	int pibot = 0;
	while(pibot != U.size() - 1){
		int mayor = buscarMayor(pibot,U.size()-1,pibot,U);
		if (mayor != pibot){
			swap(U[pibot],U[mayor]);		
			swap(P[pibot],P[mayor]);
			swap(L[pibot],L[mayor]);
		} 
		for(int i = pibot+1; i < U.size(); i++){
			Num d = U[i][pibot]/U[pibot][pibot];
			L[i][pibot] = d;
			for(int j = pibot; j < U[i].size(); j++){
				U[i][j] = U[i][j] - d*U[pibot][j];
			}
		}
		pibot++;
	}
	L = L + identidad(L.size());
	return make_tuple(P,L,U);
}

int main(){
	Matriz A = generarMatrizA();
	Lista B = generarMatrizB();
	auto t = PLU(A);
	Matriz P = get<M_P>(t);
	Matriz L = get<M_L>(t);
	Matriz U = get<M_U>(t);
	Lista Y = SustitucionProgresiva(matrizAumentada(L,P*B));
	Lista res = SustitucionRegresiva(matrizAumentada(U,Y));
	mostrarLista(res);
}

/*
int main(){
	//Matriz A = {{1,2,3},{2,3,6},{8,4,1}};
	Matriz A = {{-1},{2,-1,1},{4,1,-2}};
	Lista B = {10,9,8,7,6,5,4,3,2,1};
	auto t = PLU(A);
	Matriz P = get<M_P>(t);
	Matriz L = get<M_L>(t);
	Matriz U = get<M_U>(t);
	mostrarMatriz(P);
	mostrarMatriz(L);
	mostrarMatriz(U);
	Matriz Q = P * A;
	Matriz W = L * U;
	mostrarMatriz(Q);
	mostrarMatriz(W);
	Lista Y = SustitucionProgresiva(matrizAumentada(L,P*B));
	mostrarLista(Y);
	Lista res = SustitucionRegresiva(matrizAumentada(U,Y));
	mostrarLista(res);
}
*/