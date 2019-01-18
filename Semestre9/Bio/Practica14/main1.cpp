#include <iostream>
#include "AS1.h"

using namespace std;
				       
matrix costos = {{0,50,50,94,50},
				 {50,0,22,50,36},
				 {50,22,0,44,14},
				 {94,50,44,0,50},
				 {50,36,14,50,0}};
matrix flujos = {{0,0,2,0,3},
				 {0,0,0,3,0},
				 {2,0,0,0,0},
				 {0,3,0,0,1},
				 {3,0,0,1,0}};



int main(){
	srand(time(NULL));
	double femIni = 10;
	int numH = 4;
	int numIter = 1000;

	double evaporacion = 0.7;
	double alfa = 1;
	double beta = 1;
	double Q = 1;
	double q0 = 0.5;
	double phi = 0.5;
	


	AS as(costos, flujos, femIni, alfa, beta, Q, evaporacion, q0, phi, numH, numIter);
	as.run();
}