#include <iostream>
#include "Sustituciones.h"
#include "OperacionesMatriz.h"

using namespace std;

Matriz Gauss(Matriz A, Lista &B){
	A = matrizAumentada(A,B);
	int pibot = 0;
	while(pibot != B.size() - 1){
		int mayor = buscarMayor(pibot,B.size()-1,pibot,A);
		if (mayor != pibot) swap(A[pibot],A[mayor]);
		for(int i = pibot+1; i < B.size(); i++){
			Num d = A[i][pibot]/A[pibot][pibot];
			for(int j = pibot; j < A[i].size(); j++){
				A[i][j] = A[i][j] - d*A[pibot][j];
			}
		}
		pibot++;
	}
	return A;
}

int main(){
	Matriz A = generarMatrizA();
	Lista B = generarMatrizB();
	Matriz AA = Gauss(A,B);
	Lista res = SustitucionRegresiva(AA);
	mostrarLista(res);
}