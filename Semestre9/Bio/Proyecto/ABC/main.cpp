#include <iostream>
#include <cmath>
#include "ABC.h"
#include "Funciones.h"

using namespace std;

int main(int argc, char ** argv){
	srand(time(NULL));
	if(argc != 2){
		cout<<"Faltan Argumentos <dataFile>"<<endl;
		return 1;
	}
	string fileName(argv[1]);
	setTerrenos(fileName, zona, disponibilidadAgua);
	tamOfLastVal = zona.size() % 16;
	int numFuentes = 10;
	int numObs = 10;
	int numIter = 500;
	int tamColmena = 10;
	ABC abc;
	clock_t begin = clock();
	abc.run(funFit, funcLimit, generarFuente, numFuentes, numObs, numIter, tamColmena);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<getBin(abc.mejorGlobal.valores)<<endl;
	cout<<"Time->"<<elapsed_secs<<" s"<<endl;
}