#include <iostream>
#include "PSO.h"

using namespace std;

matrix costos{{0,1,3,4,5},
			  {1,0,1,4,8},
			  {3,1,0,5,1},
			  {4,4,5,0,2},
			  {5,8,1,2,0}};

int main(){
	srand(time(NULL));
	int numPar = 4;
	int numIter = 100;
	PSO ps(numPar, numIter);
	ps.run(costos);
}