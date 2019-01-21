#include <iostream>
#include <map>
#include "FileOp.h"
#include "Distancias.h"
#include "Algoritmos.h"

using namespace std;

int main(int argc, char ** argv){	
	if(argc != 4){
		cout<<"Faltan argumentos <valsFile> <productFile> <namesFile>"<<endl;
		return 0;
	}
	string valsFile(argv[1]);
	string productFile(argv[2]);
	string namesFile(argv[3]);
	auto songs = getBdNames(productFile);
	auto vals = getBdVals(valsFile, songs.size());
	auto names = getBdNames(namesFile);

	auto proms = getPromedios(vals);

	auto valsNorm = normalizate(vals);

	auto simMat = getSimilarityMatrix(vals);

	for(ValVec vec : vals){
		printValVec(vec);
	}

	cout<<endl;

	for(ValVec vec : valsNorm){
		printValVec(vec);
	}

	IndexType userIndex = 0;
	IndexType productIndex = 0;	

	while(true){
		for(int i = 0; i < names.size(); i++){
			cout<<i<<") "<<names[i]<<endl;
		}

		cout<<endl<<"Usuario->";
		cin>>userIndex;
		cout<<endl;

		for(int i = 0; i < songs.size(); i++){
			cout<<i<<") "<<songs[i]<<endl;
		}

		cout<<endl<<"Producto->";
		cin>>productIndex;
		cout<<endl;

		auto resN = prediccionPorProducto(simMat, valsNorm, productIndex, userIndex);

		cout<<"resN->"<<resN<<endl;

		cout<<"El usuario "<<names[userIndex]<<" daría "<<desnormalize(vals, resN, userIndex)<<" al producto "<<songs[productIndex]<<endl<<endl;
	}


	


/*
	for(ValVec v : valsNorm){
		printValVec(v);
	}

	cout<<cosenoAjustado(vals[0], vals[1], proms)<<endl;
*/

}
