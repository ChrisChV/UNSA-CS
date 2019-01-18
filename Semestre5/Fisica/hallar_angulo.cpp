#include <iostream>
#include "formulas.h"

using namespace std;

int main(){
	Number velocidadInicial;
	Number posicion;
	cout<<"Ingrese su Vo->";
	cin>>velocidadInicial;
	cout<<"Ingrese la posicion a alcanzar->";
	cin>>posicion;
	auto angulos = HallarAngulos(GRAVEDAD_TIERRA,velocidadInicial,posicion);
	cout<<"Angulos:"<<endl;
	cout<<"1->"<<get<0>(angulos)<<endl;
	cout<<"2->"<<get<1>(angulos)<<endl;
}