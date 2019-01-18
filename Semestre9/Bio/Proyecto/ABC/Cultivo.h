#ifndef CULTIVO_H
#define CULTIVO_H

#include <iostream>

using namespace std;

class Cultivo{
	public:
		Cultivo(){};
		Cultivo(double costo, double beneficio, int agua);
		void print();
		double costo;
		double beneficio;
		int agua;
};


Cultivo::Cultivo(double costo, double beneficio, int agua){
	this->costo = costo;
	this->beneficio = beneficio;
	this->agua = agua;
}

void Cultivo::print(){
	cout<<"Costo: "<<costo<<endl;
	cout<<"Beneficio: "<<beneficio<<endl;
	cout<<"Agua: "<<agua<<endl;
}

#endif