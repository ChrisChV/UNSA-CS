#ifndef SUSTITUCIONES_H
#define SUSTITUCIONES_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "OperacionesMatriz.h"

using namespace std;

Lista SustitucionRegresiva(Matriz A){ //Con Matriz Aumentada
	int j = 0;
	Lista res(A.size());
	for(int i = A.size() - 1; i >= 0; i--, j++){
		Num sum = 0;
		for(int k = 0; k < j; k++){
			sum += A[i][A[i].size() - 2 - k] * res[A[i].size() -2 -k];
		}
		res[A[i].size() -2-j] = (A[i][A[i].size()-1] - sum) / A[i][A[i].size()-2-j];

	}
	return res;
}

Lista SustitucionProgresiva(Matriz A){ //Con Matriz Aumentada
	int j = 0;
	Lista res(A.size());
	for(int i = 0; i < A.size(); i++, j++){
		Num sum = 0;
		for(int k = 0; k < j; k++){
			sum += A[i][k] * res[k];
		}
		res[j] = (A[i][A[i].size()-1] - sum) / A[i][j];
	}
	return res;
}


#endif