#ifndef DISTANCIAS_H
#define DISTANCIAS_H

#include <iostream>
#include <functional>
#include "Utils.h"

typedef function<ValType(ValVec &, ValVec &)> DistanceFunction;

ValType manhattanDistance(ValVec & x, ValVec & y){
	if(x.size() != y.size()) return 0;
	ValType res = 0;
	for(int i = 0; i < x.size(); i++){
		if(x[i] == NULL_VAL or y[i] == NULL_VAL) continue;
		else res += abs(x[i] - y[i]);
	}
	return res;
}

ValType euclideanDistance(ValVec & x, ValVec & y){
	if(x.size() != y.size()) return 0;
	ValType res = 0;
	for(int i = 0; i < x.size(); i++){
		if(x[i] == NULL_VAL or y[i] == NULL_VAL) continue;
		else res += pow(x[i] - y[i], 2);
	}
	return (ValType) sqrt(res);
}

ValType minkowskiDistance(ValVec & x, ValVec & y, int r){
	if(x.size() != y.size()) return 0;
	ValType res = 0;
	for(int i = 0; i < x.size(); i++){
		if(x[i] == NULL_VAL or y[i] == NULL_VAL) continue;
		res += pow(abs(x[i] - y[i]), r);
	}
	if(r == 2) return (ValType) sqrt(res);
	return res;
}

ValType cosenDistance(ValVec & x, ValVec & y){
	if(x.size() != y.size()) return 0;
	if((vectorModule(x) * vectorModule(y)) == 0) return -1;
	return dotProduct(x,y) / (vectorModule(x) * vectorModule(y));
}

ValType cosenDistanceSinM(ValVec & x, ValVec & y){
	return dotProduct(x,y);
}

ValType personAproxDN(ValVec & x, ValVec & y){
	ValType res = 0;	
	ValVec xx;
	ValVec yy;
	tie(xx,yy) = deleteNulls(x,y);
	ValType n = xx.size();
	cout<<"N->"<<n<<endl;
	return (ValType) (dotProduct(xx,yy) - (sumatoria(xx) * sumatoria(yy) / n)) / (sqrt(sumatoriaCuadratica(xx) - pow(sumatoria(xx),2) / n) * sqrt(sumatoriaCuadratica(yy) - pow(sumatoria(yy),2) / n));
	//ValType div = (sqrt(sumatoriaCuadratica(x) - pow(sumatoria(x),2) / n) * sqrt(sumatoriaCuadratica(y) - pow(sumatoria(y),2) / n));
	//if(div == 0) return INFINITO_NEGATIVO;
	//return (ValType) (dotProduct(x,y) - (sumatoria(x) * sumatoria(y) / n)) / div;
}

ValType personAprox(ValVec & x, ValVec & y){
	ValType res = 0;	
	//tie(xx,yy) = deleteNulls(x,y);
	//ValType n = xx.size();
	ValType n = x.size();
	//cout<<"N->"<<n<<endl;
	//return (ValType) (dotProduct(xx,yy) - (sumatoria(xx) * sumatoria(yy) / n)) / (sqrt(sumatoriaCuadratica(xx) - pow(sumatoria(xx),2) / n) * sqrt(sumatoriaCuadratica(yy) - pow(sumatoria(yy),2) / n));
	ValType div = (sqrt(sumatoriaCuadratica(x) - pow(sumatoria(x),2) / n) * sqrt(sumatoriaCuadratica(y) - pow(sumatoria(y),2) / n));
	if(div == 0) return -1;
	return (ValType) (dotProduct(x,y) - (sumatoria(x) * sumatoria(y) / n)) / div;
}

ValType cosenoAjustado(ValVec & x, ValVec & y, ValVec & proms){
	ValType temp1;
	ValType temp2;
	ValType d = 0;
	ValType t1 = 0;
	ValType t2 = 0;
	for(int i = 0; i < x.size(); i++){
		if(x[i] == NULL_VAL or y[i] == NULL_VAL) continue;
		temp1 = proms[i] - x[i];
		temp2 = proms[i] - y[i];
		d += temp1 * temp2;
		t1 += pow(temp1, 2);
		t2 += pow(temp2, 2);
	}
	ValType t = (sqrt(t1) * sqrt(t2));
	if(t == 0){
		//cout<<"Ahhhhhhhh!!! Divison entre cero"<<endl;
		return -1;
	}
	
	return (ValType)  d / t;
}

ValType desviacionEstandar(ValVec & x, ValVec & y){ //tienes que pasar los vectores con los valores que hacen match
	float cardinalidad = x.size();
	ValType res = 0;
	for(int i = 0; i < x.size(); i++){
		res += (x[i] - y[i]) / cardinalidad;
	}
	cout<<"R->"<<res<<endl;
	cout<<cardinalidad<<endl;
	return res;
}

#endif