#include <iostream>
#include "PSO.h"

using namespace std;

int main(){
	srand(time(NULL));
	int numPar = 4;
	int numIter = 50;
	PSO ps(numPar, numIter);
	ps.run();
}