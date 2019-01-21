#include <iostream>
#include "clonalg.h"

using namespace std;

TypeVal funFit(vector<TypeVal> valores){
	return  pow(valores[0],2) + pow(valores[1], 2);
}

int main(){
	srand(time(NULL));
	Clonalg cl;
	int iteraciones = 1000;
	int tamPobl = 4;
	int numRandoms = 2;
	TypeVal mutationFactor = -2.5;
	TypeVal cloneRate = 0.5;
	cl.run(iteraciones, tamPobl, numRandoms, mutationFactor, cloneRate, funFit);
}