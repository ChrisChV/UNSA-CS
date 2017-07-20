#ifndef CROMOSOMA_H
#define CROMOSOMA_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>

using namespace std;

class Cromosoma{
	public:
		Cromosoma();
		void del(){
			delete[] vector;
		}
		Cromosoma(int size);
		Cromosoma(int size, char * a1, char * a2);
		int binaryToDecimal();
		void print();
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
	vector = new char[size + 1];
	for(int i = 0; i < size; i++){
		random = rand() % 2;
		stemp = to_string(random);
		vector[i] = stemp[0];
	}
	vector[size] = '\0';
}

Cromosoma::Cromosoma(int size, char * a1, char * a2){
	this->size = size;
	vector = new char[size + 1];
	int size1 = strlen(a1);
	memcpy(vector,a1,size1);
	memcpy(&vector[size1],a2,strlen(a2));
	vector[size] = '\0';
}

int Cromosoma::binaryToDecimal(){
	int res = 0;
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
