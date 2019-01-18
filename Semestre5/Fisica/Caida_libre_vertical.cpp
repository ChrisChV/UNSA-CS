#include <iostream> 
#include <fstream>
#include "formulas.h"

#define PRECISION 0.1

using namespace std;

int main(){
	Number velocidadInicial;
	string f;
	cout<<"Ingres el Vo->";
	cin>>velocidadInicial;
	cout<<endl<<"Escriba el nombre del archivo"<<endl;	
	cin>>f;
	string file = f + ".csv";
	ofstream archivo(file);
	auto tiempoVuelo = TiempoVuelo(GRAVEDAD_TIERRA,velocidadInicial);
	auto elevacion = Elevacion(GRAVEDAD_TIERRA, velocidadInicial);
	archivo<<"\"TiempoVuelo\","<<tiempoVuelo<<endl;
	archivo<<"\"Elevacion\","<<elevacion<<endl;
	archivo<<"\"Tiempo\",\"Posicion\",\"Velocidad\""<<endl;
	for(Number time = 0; tiempoVuelo > 0; time += PRECISION, tiempoVuelo -= PRECISION){
		archivo<<time<<","<<PosicionEnCaida(GRAVEDAD_TIERRA,velocidadInicial,time,0)<<","
				<<VelocidadEnCaida(GRAVEDAD_TIERRA,velocidadInicial,time)<<endl;
	}
	cout<<"Archivo generado correctamente"<<endl;
	archivo.close();
}