#ifndef OPERACIONESMATRIZ_H
#define OPERACIONESMATRIZ_H

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

typedef long double Num;
typedef vector<vector<Num>> Matriz;
typedef vector<Num> Lista;
typedef vector<Num(*)(Lista)> ListaFunciones;
typedef vector<ListaFunciones> MatrizFunciones;

Num Distancia(Lista r_anterior, Lista r_actual){
	Num sum = 0;
	for(int i = 0; i < r_anterior.size(); i++){
		sum += pow(r_anterior[i] - r_actual[i],2);
	}
	return sqrt(sum);
}	

bool ErrorAbsoluto(Lista r_anterior, Lista r_actual, Num presicion){
	if(Distancia(r_anterior,r_actual) < presicion) return true;
	return false;
}

void mostrarLista(Lista A){
	for(int i = 0; i < A.size(); i++){
		cout<<A[i]<<endl;
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

void mostrarMatriz(Matriz A){
	for(int i = 0; i < A.size(); i++){
		for(int j = 0; j < A[i].size(); j++){
			cout<<A[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

int buscarMayor(int ini, int fin, int j, Matriz A){
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


Lista operator * (Matriz a, Lista b){
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

Matriz operator * (Matriz a, Matriz b){
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

Lista operator *(Lista a, Num b){
	Lista res = a;
	for(int i = 0; i < a.size(); i++){
		res[i] = b * a[i];
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

Lista operator +(Lista a, Lista b){
	Lista res = a;
	for(int i = 0; i < a.size(); i++){
		res[i] = a[i] + b[i];
	}
	return res;
}

Matriz operator -(Matriz a, Matriz b){
	int n = a.size();
	Matriz res = a;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			res[i][j] = a[i][j] - b[i][j];
		}
	}
	return res;
}

Lista operator -(Lista a, Lista b){
	Lista res = a;
	for(int i = 0; i < a.size(); i++){
		res[i] = a[i] - b[i];
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

Matriz zeros(int fil, int col){
	Matriz res(fil);
	for(int i = 0; i < fil; i++){
		res[i] = vector<Num>(col);
	}
	return res;
}

Lista createLista(Num num, Num n){
	Lista res(n);
	for(int i = 0; i < n; i++){
		res[i] = num;
	}
	return res;
}

Matriz createMatriz(Num num, Num n){
	Matriz res = zeros(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			res[i][j] = num;
		}
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

Num frobenius(Matriz A){
	Num sum = 0;
	for(int i = 0; i < A.size(); i++){
		for(int j = 0; j < A[i].size(); j++){
			sum += pow(abs(A[i][j]),2);
		}
	}
	return sqrt(sum);
}

Num normInf(Matriz A){
	bool flag = false;
	Num norm = 0;
	for(int i = 0; i < A.size(); i++){
		Num sum = 0;
		for(int j = 0; j < A[i].size(); j++){
			sum += A[i][j];
		}
		if(!flag){
			norm = sum;
			flag = true;
		}
		else if(sum > norm) norm = sum;
	}
	return norm;
}

Num normInf(Lista A){
	bool flag = false;
	Num norm = 0;
	for(int i = 0; i < A.size(); i++){
		if(!flag){
			norm = A[i];
			flag = true;
		}
		else if(A[i] > norm) norm = A[i];
	}
	return norm;
}

Matriz Hilbert(int n){
	Matriz res = zeros(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n ; j++){
			res[i][j] = 1.0 / ((i+1)+(j+1)-1);
		}
	}
	return res;
}

#endif