#ifndef CROMOSOMA_H
#define CROMOSOMA_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "sad.h"

using namespace std;

class Cromosoma{
	public:
		Cromosoma();
		Cromosoma(int size);
		Cromosoma(int size, char * a1, char * a2);
		void push_back(char c);
		void print();
		Val getFenotipo();
		char * vector;
		int size;
};

Cromosoma::Cromosoma(){
	vector = nullptr;
	size = 0;
}

Cromosoma::Cromosoma(int size){
	this->size = size;
	int random = 0;
	string stemp = "";
	for(int i = 0; i < size; i++){
		random = rand() % 2;
		stemp = to_string(random);
		push_back(stemp[0]);
	}
}

Cromosoma::Cromosoma(int size, char * a1, char * a2){
	this->size = size;
	vector = (char *) malloc(size);
	int size1 = strlen(a1);
	memcpy(vector,a1,size1);
	memcpy(&vector[size1],a2,strlen(a2));
}

void Cromosoma::push_back(char c){
	size++;
	if(size == 1) vector = (char *) malloc(size);
	else vector = (char *) realloc(vector, size);
	vector[size - 1] = c;
}


Val Cromosoma::getFenotipo(){
	Val res = 0;
	for(int i = 0; i < size; i++){
		if(vector[i] == '1') res += pow(2,size - 1 - i);
	}
	return res;
}

void Cromosoma::print(){
	for(int i = 0; i < size; i++){
		cout<<vector[i]<<" ";
	}
}

#endif