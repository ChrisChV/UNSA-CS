#include <iostream>
#include <cmath>
#include "ABC.h"


using namespace std;

TypeVal funFit(vector<TypeVal> valores){
	return pow(valores[0],2) + pow(valores[1],2);
}

int main(){
	srand(time(NULL));
	int tamColmena = 6;
	int numVal = 2;
	int numFuentes = 3;
	int numObs = 3;
	int numIter = 100;
	ABC abc;
	abc.run(funFit, numVal, numFuentes, numObs, numIter, tamColmena);
}