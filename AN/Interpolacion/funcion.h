#ifndef FUNCION_H
#define FUNCION_H
#include <iostream>
#include <cmath>
#include "Punto.h"

#include "OperacionesMatriz.h"
using namespace std;

Num fun(Num x){
return 2.000000+1.000000* (x-1.000000)+-0.500000* (x-1.000000)*(x-3.000000);
}

vector<Punto> getPuntos(){
vector<Punto> res;
res.push_back(Punto(1,2));
res.push_back(Punto(3,4));
res.push_back(Punto(5,2));
return res;
}
#endif