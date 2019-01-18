#include <iostream>
#include <fstream>

using namespace std;

void contar(){
	long suma = 0;
	ifstream archivo("Resultados6/total");
	long actual;
	while(archivo>>actual){
		suma += actual;
	}
	cout<<suma<<endl;
}

void ordenar(){
	ofstream archivo("Resultados6/total");
	for(int i = 0; i < 32; i++){
		string file = "Resultados6/total000" + to_string(i);
		ifstream archivo2(file.c_str());
		string actual;
		archivo2>>actual;
		archivo<<actual<<endl;
		archivo2.close();
	}
	archivo.close();
}

int main(){
	ordenar();
	contar();
}