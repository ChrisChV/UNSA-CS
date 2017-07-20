#include <iostream>
#include "Interface/Interface.h"

using namespace std;

int main(int argc, char ** argv){
	if(argc == 1){
		cout<<"Faltan Argumentos <-c:cifrar/-d:descrifrar>"<<endl;
		return 0;
	}
	string type(argv[1]);
	if(type == "-c"){
		if(argc != 6){
			cout<<"Faltan Argumentos -c <imgSecreta> <stegoImg1> <stegoImg2> <outKeyfile>"<<endl;
			return 0;
		}
		string img(argv[2]);
		string stegoImg1(argv[3]);
		string stegoImg2(argv[4]);
		string keyFile(argv[5]);
		string key = crifrar_ocultar(img,stegoImg1,stegoImg2,keyFile);
		cout<<"Cifrado correctamente"<<endl;
	}
	else if(type == "-d"){
		if(argc != 3){
			cout<<"Faltan Argumentos -d <keyFile>"<<endl;
			return 0;
		}
		string keyFile(argv[2]);
		CImg<int> img = revelar_descrifrar(keyFile);
		cout<<"Descifrado correctamente"<<endl;
		img.display();
	}
	else{
		cout<<type<<endl;
		//count<<"No se reconoce "<<type<<endl;

		return 0;
	}
}