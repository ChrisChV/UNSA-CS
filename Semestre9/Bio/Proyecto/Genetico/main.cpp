#include <iostream>
#include <fstream>
#include "Funciones.h"

using namespace std;

void setTerrenos(string fileName, vector<Terreno> & terrenos, int & dispAgua){
	ifstream inFile(fileName);
	int numTerrenos = 0;
	int area = 0;
	double distancia = 0;
	double rendimiento = 0;
	int agua = 0;
	double costo = 0;
	double beneficio = 0;

	inFile>>numTerrenos;
	inFile>>dispAgua;

	for(int i = 0; i < numTerrenos; i++){
		inFile>>area;
		inFile>>distancia;
		inFile>>rendimiento;
		inFile>>agua;
		inFile>>costo;
		inFile>>beneficio;
		terrenos.push_back(Terreno(area,rendimiento, distancia, costo, beneficio, agua));
	}
}

int main(int argc, char ** argv){
	srand(time(NULL));
	if(argc != 2){
		cout<<"Faltan Argumentos <dataFile>"<<endl;
		return 1;
	}
	string fileName(argv[1]);
	setTerrenos(fileName, zona, disponibilidadAgua);
	int tamP = 10;
	int tamCrom = zona.size();
	int numIter = 500;
	float probCruz = 0.9;
	float probMut = 0.1;
	AlgoGen AG(tamP, tamCrom, numIter, probCruz, probMut, funcFitness, funcRouleteSelect,
				 funcCrossTwoPoints, mutFunc, nextGen, funcLimit, initPoblacion);
	clock_t begin = clock();
	AG.run();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"Time->"<<elapsed_secs<<" s"<<endl;
}

