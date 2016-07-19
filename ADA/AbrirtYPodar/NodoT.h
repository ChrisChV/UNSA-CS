#ifndef NODOT_H
#define NODOT_H

#include <iostream>
#include <vector>

using namespace std;

class NodoT{
	public:
		NodoT();
		NodoT(int);
		NodoT(NodoT *,int);
		int trabajo;
		vector<NodoT*> padres;
};

NodoT::NodoT(){
	trabajo = 0;
}

NodoT::NodoT(int t){
	trabajo = t;
}

NodoT::NodoT(NodoT * n, int t){
	trabajo = t;
	padres.push_back(n);
}

#endif