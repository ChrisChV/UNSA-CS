#include <iostream>
#include "PSO.h"

using namespace std;

int main(){
	srand(time(NULL));
	int numPar = 6;
	int numIter = 1000;
	PSO ps(numPar, numIter);
	ps.run();
}