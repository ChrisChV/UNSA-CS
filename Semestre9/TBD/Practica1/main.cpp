#include <iostream>
#include <fstream>
#include <cmath>
#include "Distancias.h"
#include "Algoritmos.h"
#include "FileOp.h"

using namespace std;

int main(int argc, char ** argv){
	
	if(argc != 4){
		cout<<"Faltan argumentos <valsFile> <namesFile> <pelis>"<<endl;
		return 0;
	}
	string valsFileName(argv[1]);
	string namesFileName(argv[2]);
	string pelisFileName(argv[3]);
	vector<string> names = getBdNames(namesFileName);
	vector<ValVec> vals = getBdVals(valsFileName, names.size());
	vector<string> pelis = getBdNames(pelisFileName);
	

	
	//Print
	/*
	for(string s : names) cout<<s<<"\t";
	cout<<endl;
	for(int i = 0; i < vals.size(); i++){
		for(int j = 0; j < vals.front().size(); j++){
			cout<<vals[j][i]<<"\t";
			if(j == 0) cout<<"\t";
		}
		cout<<endl;
	}
	*/
	//End Print
	while(true){
		for(int i = 0; i < names.size(); i++){
			cout<<i<<") "<<names[i]<<endl;
		} 

		cout<<"1 Distancia Manhattan"<<endl;
		cout<<"2 Distancia Euclidiana"<<endl;
		cout<<"3 Distancia Minkowski"<<endl;
		cout<<"4 Distancia Coseno"<<endl;
		cout<<"5 Aproximacion de Person"<<endl;
		cout<<"6 KNN"<<endl;
		cout<<"7 Proyeccion"<<endl;
		cout<<"8 Recomendacion"<<endl;
		cout<<"9 Agregar Usuario"<<endl;

		int option = 0;
		cout<<"Opcion->";
		cin>>option;

		int a = 0;
		int b = 0;
		int k = 0;

		if(option == 6){
			cout<<"De quien->";
			cin>>a;
			cout<<"K->";
			cin>>k;
		}
		else if(option == 7 or option == 8){
			cout<<"De quien->";
			cin>>a;
		}
		else if(option == 9){
			for(int i = 0; i < pelis.size(); i++){
				cout<<i<<") "<<pelis[i]<<endl;
			}
			vals.push_back(ValVec(pelis.size(),0));
			string newName;
			int peliId = 0;
			ValType puntaje;
			cout<<"Nombre del usuario->";
			cin>>newName;
			names.push_back(newName);
			while(true){
				cout<<"Peli a puntuar->";
				cin>>peliId;
				if(peliId == -1) break;
				cout<<"Puntaje->";
				cin>>puntaje;
				vals.back()[peliId] = puntaje;
			}
			for(ValType tt : vals.back()){
				cout<<tt<<endl;
			}
			continue;
		}
		else{
			cout<<"Entre quines?? (indices)"<<endl;
			cout<<"A->";
			cin>>a;
			cout<<"B->";
			cin>>b;
		}
		ValType res = 0;

		//ValVec aa = {2	,5,	0,	5,	3,	4	,5,	4,	3,	3,	1,	4,	1,	4,	0,	5,	2,	4,	4,5	,0,	3,	3,	3,	1};
		//ValVec bb = {0	,3,	3,	3,	4,	4,	 0,	3,	1,	0,	2,	3,	0,	4,	0,	2,	2,	3,	0,4	,0,	5,	4,	0,	2};

	//	ValVec aa = {5,	5,	3,	4	,	4,	3,	1,	4,	4,	5,	2,	4,5,	3,	3,	1};
	//	ValVec bb = {3,	3,	4,	4	,	3,	1,	2,	3,	4,	2,	2,	3,4,	5,	4,	2};



		switch(option){
			case 1:{
				res = manhattanDistance(vals[a], vals[b]);
				cout<<"Res: "<<res<<endl;
				break;
			}
			case 2:{
				res = euclideanDistance(vals[a], vals[b]);
				cout<<"Res: "<<res<<endl;
				break;
			}
			case 3:{
				cout<<"Valor de R?? ->";
				int r = 0;
				cin>>r;
				res = minkowskiDistance(vals[a], vals[b], r);
				cout<<"Res : "<<res<<endl;
				break;
			}
			case 4:{
				res = cosenDistance(vals[a], vals[b]);
				cout<<"Res: "<<res<<endl;
				break;
			}
			case 5:{
				res = personAproxDN(vals[a], vals[b]);
				cout<<"Res: "<<res<<endl;
				break;
			}
			case 6:{
				auto vecRes = KNN(vals, a, k, cosenDistance, [] (KNNTuple a, KNNTuple b){return get<1>(a) > get<1>(b);});
				for(KNNTuple knnT : vecRes){
					cout<<names[get<0>(knnT)]<<" "<<get<1>(knnT)<<" "<<get<2>(knnT)<<endl;
				}
				break;
			}
			case 7:{
				cout<<endl;
				for(int i = 0; i < pelis.size(); i++){
					cout<<i<<") "<<pelis[i]<<endl;
				} 
				int peliId = 0;
				cout<<endl;
				cout<<"Cual peli->";
				cin>>peliId;
				auto vecRes = KNN(vals, a, personAproxDN, [] (KNNTuple a, KNNTuple b){return get<1>(a) > get<1>(b);});
				ValVec valores;
				for(KNNTuple knnT : vecRes){
					ValType findRes = vals[get<0>(knnT)][peliId];
					valores.push_back(findRes);
				}

				ValType ratio = 0.6;
				for(auto iter = vecRes.begin(); iter != vecRes.end(); ++iter){
					if(get<1>(*iter) < ratio){
						vecRes.erase(iter, vecRes.end());
						break;	
					} 
				}

				ValType sum = 0;
				for(IndexType i = 0; i < vecRes.size(); i++){
					if(valores[i] == NULL_VAL) continue;
					sum += get<1>(vecRes[i]);
				}
				for(IndexType i = 0; i < vecRes.size(); i++){
					if(valores[i] == NULL_VAL) continue;
					get<2>(vecRes[i]) = get<1>(vecRes[i]) / sum;
				}
				for(KNNTuple knnT : vecRes){
					cout<<names[get<0>(knnT)]<<" "<<get<1>(knnT)<<" "<<get<2>(knnT)<<endl;
				}

				res = porcentajeProyectado(vecRes, valores);
				cout<<"EL porcentaje proyectado para la peli "<<pelis[peliId]<<" es "<<res<<endl;
				break;
			}
			case 8:{
				cout<<endl;
				for(int i = 0; i < pelis.size(); i++){
					cout<<i<<") "<<pelis[i]<<endl;
				} 
				int peliId = 0;
				cout<<endl;
				auto vecRes = KNN(vals, a, 1, personAproxDN, [] (KNNTuple a, KNNTuple b){return get<1>(a) > get<1>(b);});
				for(KNNTuple knnT : vecRes){
					cout<<names[get<0>(knnT)]<<" "<<get<1>(knnT)<<" "<<get<2>(knnT)<<endl;
				}
				auto indexRes = recomendNearest(vals[get<0>(vecRes[0])]);
				cout<<"Pelis recomendadas:"<<endl;
				for(int i : indexRes){
					if(vals[a][i] != NULL_VAL) continue;
					cout<<pelis[i]<<endl;
				}
			}
		}
	}
}	