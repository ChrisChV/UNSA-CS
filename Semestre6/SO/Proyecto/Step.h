#ifndef STEP_H
#define STEP_H

#include <iostream>
#include <vector>

using namespace std;

enum HW_ENUM{PROCESADOR, ES};

class Step{
	private:

	public:	
		bool hardware;
		int bucle;
		vector<string> algoritmos;	
		Step(){};
};



#endif