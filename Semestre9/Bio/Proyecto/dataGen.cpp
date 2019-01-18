#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

int main(int argc, char ** argv){
	srand(time(NULL));

	int numTerrenos = 0;
	int disponibilidadAgua = 0;
	int areaMin = 0;
	int areaMax = 0;
	double distanciaMin = 0;
	double distanciaMax = 0;
	int reqAguaMin = 0;
	int reqAguaMax = 0;
	double costoMin = 0;
	double costoMax = 0;
	double rendimientoMin = 0;
	double rendimientoMax = 0;



	if(argc == 1){
		cout<<"Número de Terrenos: ";
		cin>>numTerrenos;
		cout<<"Disponibilidad del Agua: ";
		cin>>disponibilidadAgua;
		cout<<"Rango de las Areas"<<endl;
		cout<<"Min: ";
		cin>>areaMin;
		cout<<"Max: ";
		cin>>areaMax;
		if(areaMin > areaMax){
			cout<<"Error en el rango"<<endl;
			return 1;
		}
		cout<<"Rango de las Distancias"<<endl;
		cout<<"Min: ";
		cin>>distanciaMin;
		cout<<"Max: ";
		cin>>distanciaMax;
		if(distanciaMin > distanciaMax){
			cout<<"Error en el rango"<<endl;
			return 1;
		}
		cout<<"Rango del requerimiento del agua"<<endl;
		cout<<"Min: ";
		cin>>reqAguaMin;
		cout<<"Max: ";
		cin>>reqAguaMax;
		cout<<"Rango de los costos"<<endl;
		cout<<"Min: ";
		cin>>costoMin;
		cout<<"Max: ";
		cin>>costoMax;
		cout<<"Rango de los rendimientos"<<endl;
		cout<<"Min: ";
		cin>>rendimientoMin;
		cout<<"Max: ";
		cin>>rendimientoMax;
	}
	else if(argc == 2){
		string fileName(argv[1]);
		ifstream inFile(fileName);
		if(inFile.fail()){
			cout<<"El archivo "<<fileName<<" no existe"<<endl;
			return 1;
		}
		inFile>>numTerrenos;
		inFile>>disponibilidadAgua;
		inFile>>areaMin;
		inFile>>areaMax;
		if(areaMin > areaMax){
			cout<<"Error en el rango"<<endl;
			return 1;
		}
		inFile>>distanciaMin;
		inFile>>distanciaMax;
		if(distanciaMin > distanciaMax){
			cout<<"Error en el rango"<<endl;
			return 1;
		}
		inFile>>reqAguaMin;
		inFile>>reqAguaMax;
		inFile>>costoMin;
		inFile>>costoMax;
		inFile>>rendimientoMin;
		inFile>>rendimientoMax;
	}
	else{
		cout<<"Demasiados Argumentos"<<endl;
		return 1;
	}

	ofstream outFile("outData");

	int area = 0;
	double distancia = 0;
	int agua = 0;
	double costo = 0;
	double factorAgua = 0;
	double rendimiento = 0;
	double precioCompra = 0;
	double precioVenta = 0;
	double beneficio = 0;

	outFile<<numTerrenos<<" "<<disponibilidadAgua<<endl;

	for(int i = 0; i < numTerrenos; i++){
		area = rand() % (areaMax - areaMin + 1) + areaMin;
		distancia = distanciaMin + static_cast<double> (rand()) / (static_cast<double> (RAND_MAX/(distanciaMax - distanciaMin)));
		agua = rand() % (reqAguaMax - reqAguaMin + 1) + reqAguaMin;
		costo = costoMin + static_cast<double> (rand()) / (static_cast<double> (RAND_MAX/(costoMax - costoMin)));
		factorAgua = (double) (agua - reqAguaMin) / (double) (reqAguaMax - reqAguaMin);
		rendimiento = rendimientoMin + static_cast<double> (rand()) / (static_cast<double> (RAND_MAX/(rendimientoMax - rendimientoMin)));
		//rendimiento = rendimiento * factorAgua + rendimientoMin;
		rendimiento = rendimiento / rendimientoMax;
		precioCompra = costo * (1.0 / rendimiento);
		precioVenta = precioCompra + static_cast<double> (rand()) / (static_cast<double> (RAND_MAX/(1 - precioCompra)));
		beneficio = rendimiento * precioVenta;

		outFile<<area<<" "<<distancia<<" "<<rendimiento<<" "<<agua<<" "<<costo<<" "<<beneficio<<endl;

	}

	cout<<"Resultados guardados en outData"<<endl;
}