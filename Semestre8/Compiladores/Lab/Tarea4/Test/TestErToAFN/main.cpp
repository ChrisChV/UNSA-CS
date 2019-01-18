#include <iostream>
#include "../../ErToAFN/compiladorErToAFN.h"
#include "../../Error/error.h"
#include "../../Utils/utils.h"

using namespace std;

int main(int argc, char ** argv)
{
	try{
		if(argc != 3){
			cout<<"Faltan argumentos <fileIn> <fileOut>"<<endl;
			return 0;
		}
		string fileName(argv[1]);
		string fileOut(argv[2]);
		string er = leerER(fileName);
		CompiladorErToAuto comp;
		auto test = comp.run(er,fileOut);
		for(Estado * estado : test.estados){
			cout<<estado->id<<endl;
			for(Transicion transicion : estado->transiciones){
				char entrada;
				Estado * estado1;
				Estado * estado2;
				tie(estado1,entrada,estado2) = transicion;
				cout<<entrada<<"->"<<estado2->id<<" ";
			}
			cout<<endl;
		}
	}
	catch(Error e){
		manejarError(e);
	}
	
}
