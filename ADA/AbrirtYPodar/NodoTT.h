#ifndef NODOTT_H
#define NODOTT_H

#include <iostream>
#include "NodoT.h"

using namespace std;

class NodoTT:public NodoT{
	public:
		NodoTT();
		NodoTT(int);
		void insert(NodoTT *);
		int nivel;
};

NodoTT::NodoTT(){
	nivel = 0;
	trabajo = 0;
}

NodoTT::NodoTT(int t){
	nivel = 0;
	trabajo = t;
}

void NodoTT::insert(NodoTT * n){
	padres.push_back(n);
	if(n->nivel >= this->nivel){
		this->nivel = n->nivel + 1;
	}
}

#endif