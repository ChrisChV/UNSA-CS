#include <iostream>
#include <fstream>
#include "formulas.h"

#define PRESICION 0.1

using namespace std;

int main(){
	Number velocidadInicial;
	Number angulo;
	string f;
	cout<<"Ingrese su Vo->";
	cin>>velocidadInicial;
	cout<<"Ingrese el angulo->";
	cin>>angulo;
	cout<<endl<<"Escriba el nombre del archivo"<<endl;	
	cin>>f;
	string file = f + ".csv";
	ofstream archivo(file);
	auto tiempoVuelo = TiempoVuelo(GRAVEDAD_TIERRA,velocidadInicial,angulo);
	auto elevacion = Elevacion(GRAVEDAD_TIERRA, velocidadInicial,angulo);
	auto alcance = AlcanceConAngulo(GRAVEDAD_TIERRA,velocidadInicial,angulo);
	auto velX = VelocidadInicialEnX(velocidadInicial,angulo);
	archivo<<"\"TiempoVuelo\","<<tiempoVuelo<<endl;
	archivo<<"\"Elevacion\","<<elevacion<<endl;
	archivo<<"\"Alcance\","<<alcance<<endl;
	archivo<<"\"Velocidad en X\","<<velX<<endl;
	archivo<<"\"Tiempo\",\"Posicion en X\",\"Posicion en Y\",\"Velocidad en Y\""<<endl;
	for(Number time = 0; tiempoVuelo > 0; time += PRESICION, tiempoVuelo-= PRESICION){
		archivo<<time<<","<<PosicionEnX(velocidadInicial,time,angulo,0)<<","
		        <<PosicionEnY(GRAVEDAD_TIERRA,velocidadInicial,time,angulo,0)<<","
		          <<VelocidadEnY(GRAVEDAD_TIERRA,velocidadInicial,time,angulo)<<endl;
	}
	cout<<"Archivo generado correctamente"<<endl;
	archivo.close();
}