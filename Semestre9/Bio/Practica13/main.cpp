#include <iostream>
#include "AS1.h"

using namespace std;
				        
matrix costos = {{0,12,6,4},
				 {12,0,6,8},
				 {6,6,0,7} ,
				 {4,8,7,0}};

matrix flujos = {{0,3,8,3},
   				 {3,0,2,4},
   				 {8,2,0,5},
   				 {3,4,5,0}};

int main(){
	srand(time(NULL));
	int numH = 4;
	int numIter = 1000;
	double ferMin = 1;
	double ferMax = 10;

	double evaporacion = 0.999;
	double alfa = 1;
	double beta = 1;
	double Q = 1;


	AS as(costos, flujos, alfa, beta, Q, evaporacion, ferMax, ferMin, numH, numIter);
	as.run();
}