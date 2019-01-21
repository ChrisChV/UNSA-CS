#ifndef TERRENO_H
#define TERRENO_H

#include <iostream>
#include "Cultivo.h"

using namespace std;

class Terreno{
	public:
		Terreno(){};
		Terreno(int area, double rendimiento, Cultivo cultivo);
		Terreno(int area, double rendimiento, double distancia, double costo, double beneficio, int agua);
		void print();
		double getFitness();
		int area;
		double rendimiento;
		double distancia;
		Cultivo cultivo;
};

Terreno::Terreno(int area, double rendimiento, Cultivo cultivo){
	this->area = area;
	this->rendimiento = rendimiento;
	this->cultivo = cultivo;
}

Terreno::Terreno(int area, double rendimiento, double distancia, double costo, double beneficio, int agua){
	this->area = area;
	this->rendimiento = rendimiento;
	this->distancia = distancia;
	this->cultivo = Cultivo(costo, beneficio, agua);
}

void Terreno::print(){
	cout<<"Area: "<<area<<endl;
	cout<<"Rendimiento: "<<rendimiento<<endl;
	cout<<"Distancia: "<<distancia<<endl;
	cultivo.print();
	cout<<endl;
}

double Terreno::getFitness(){
	double res = 0;
	res = (area * cultivo.beneficio * rendimiento) / (cultivo.costo * distancia);
	return res;
}

#endif