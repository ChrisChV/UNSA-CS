#include <iostream>
#include "regLn.h"

int main(){
	//Matriz X = {{1,1,1,1,1},{3,6,2,1,5}};
	//M_Columna Y = {{6,11,3,1,9}};
	Matriz X = {{1,1,1,1,1},{1,2,3,4,5}};
	M_Columna Y = {{1,2,3,4,5}};
	regresion(X,Y,0.0001);
}