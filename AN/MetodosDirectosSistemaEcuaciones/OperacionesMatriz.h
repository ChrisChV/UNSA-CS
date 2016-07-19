#ifndef OPERACIONESMATRIZ_H
#define OPERACIONESMATRIZ_H

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

typedef long double Num;
typedef vector<vector<Num>> Matriz;
typedef vector<Num> Lista;



void mostrarLista(Lista &A){
	for(int i = 0; i < A.size(); i++){
		cout<<A[i]<<" ";
	}
	cout<<endl<<endl;;
}

Matriz matrizAumentada(Matriz A, Lista B){
	Matriz res = A;
	for(int i = 0; i < B.size(); i++){
		res[i].push_back(B[i]);
	}
	return res;
}

void mostrarMatriz(Matriz &A){
	for(int i = 0; i < A.size(); i++){
		for(int j = 0; j < A[i].size(); j++){
			cout<<A[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

int buscarMayor(int ini, int fin, int j, Matriz &A){
	int mayor = abs(A[ini][j]);
	int index = ini;
	for(int i = ini +1; i <= fin; i++){
		if(mayor < abs(A[i][j])){
			mayor = abs(A[i][j]);
			index = i;
		} 
	}
	return index;
}

Lista operator *(Matriz a, Lista b){
	int n = a.size();
	Lista res = b;
	for(int i = 0; i < n; i++){
		for(int k = 0; k < n; k++){
			Num sum = 0;
			for(int j = 0; j < n; j++){
				sum += a[i][j] * b[j];
			}
			res[i] = sum;
		}
	}
	return res;
}

Matriz operator *(Matriz a, Matriz b){
	int n = a.size();
	Matriz res = a;
	for(int i = 0; i < n; i++){
		for(int k = 0; k < n; k++){
			Num sum = 0;
			for(int j = 0; j < n; j++){
				sum += a[i][j] * b[j][k];
			}
			res[i][k] = sum;
		}
	}
	return res;
}

Matriz operator +(Matriz a, Matriz b){
	int n = a.size();
	Matriz res = a;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

Matriz zeros(int n){
	Matriz res(n);
	for(int i = 0; i < n; i++){
		res[i] = vector<Num>(n);
	}
	return res;
}

Matriz identidad(int n){
	Matriz res(n);
	for(int i = 0; i < n; i++){
		res[i] = vector<Num>(n);
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i == j) res[i][j] = 1;
		}
	}
	return res;
}

Matriz generarMatrizA(){
	Matriz res = zeros(10);
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			if(i==j) res[i][j] = -1;
			else if(abs(i-j)<2) res[i][j] = 1;
			else res[i][j] = 0;
		}
	}
	return res;
}

Lista generarMatrizB(){
	Lista res;
	for(int i = 0; i < 10; i++){
		res.push_back(10-i);
	}
	return res;
}

#endif