#include <iostream>
#include "AS2.h"

using namespace std;
				       
matrix costos = {{0,35,71,99,71,75,41},
				 {35,0,42,80,65,82,47},
				 {71,42,0,45,49,79,55},
				 {99,80,45,0,36,65,65},
				 {71,65,49,36,0,31,32},
				 {75,82,79,65,31,0,36},
				 {41,47,55,65,32,36,0}};

matrix flujos = {{0,2,0,0,0,0,2},
				 {2,0,3,0,0,1,0},
				 {0,3,0,0,0,1,0},
				 {0,0,0,0,3,0,1},
				 {0,0,0,3,0,0,0},
				 {0,1,1,0,0,0,0},
				 {2,0,0,1,0,0,0}};



int main(){
	srand(time(NULL));
	double femIni = 10;
	int numH = 4;
	int numIter = 1000;
	int numEstIter = 20;

	double evaporacion = 0.7;
	double alfa = 1;
	double beta = 1;
	double Q = 1;
	


	AS as(costos, flujos, femIni, alfa, beta, Q, evaporacion, numH, numIter, numEstIter);
	as.run();
}