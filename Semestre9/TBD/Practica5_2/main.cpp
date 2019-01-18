#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <thread>
#include "Distancias.h"
#include "FileOp.h"
#include "Algoritmos.h"
#include "MyTime.h"

using namespace std;

#define NUM_THREADS 4


typedef int MovieId;
typedef string MovieName;
typedef string MovieCategories;
typedef tuple<MovieName, MovieCategories> MovieRegister;

typedef int UserId;
typedef ValType Valoration;
typedef tuple<MovieId, Valoration> InterRegister;

typedef map<MovieId, Valoration> InterRegisterMap;

MyTime mytime;


void printInters(InterRegisterMap interMap){
	for(auto iter = interMap.begin(); iter != interMap.end(); ++iter){
		cout<<iter->first<<" "<<iter->second<<endl;
	}
}

void printMovie(MovieRegister movie){
	cout<<get<0>(movie)<<" "<<get<1>(movie)<<endl;
}

int main(){
	cout<<"Cargando Base de Datos..."<<endl;
	mytime.init();
	
	auto bdMovies = getBd("../ml/ml/movies.csv", ';');
	auto bdInter = getBd("../ml/ml/ratings.csv", ',');
	
	mytime.end();
	cout<<"Done->";
	mytime.print();

	UserId id = 0;

	cout<<"Generando estructura Movies..."<<endl;
	mytime.init();
	map<MovieId, MovieRegister> movieVec;
	for(auto iter = bdMovies.begin(); iter != bdMovies.end(); ++iter){
		id = stoi((*iter)[0]);
		movieVec[id] = make_tuple((*iter)[1], (*iter)[2]);
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();


	cout<<"Generando estructura InterbyUser..."<<endl;
	mytime.init();

	vector<InterRegisterMap> valsByUser(671);
	Valoration val = 0;	

	MovieId id2 = 0;

	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		id = stoi((*iter)[0]) - 1;
		id2 = stoi((*iter)[1]);
		val = stof((*iter)[2]);
		valsByUser[id][id2] = val;
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();
	
	cout<<"Generando estructura InterbyProduct..."<<endl;
	mytime.init();

	map<MovieId,map<UserId,ValType>> valsByProduct;
	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		id = stoi((*iter)[0]) - 1;
		id2 = stoi((*iter)[1]);
		val = stof((*iter)[2]);
		valsByProduct[id2][id] = val;
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();

	bdInter.clear();
	bdMovies.clear();

	cout<<"Normalizando Datos..."<<endl;
	mytime.init();
	auto normVals = normalizate(valsByUser);
	mytime.end();
	cout<<"Done->";
	mytime.print();	

	UserId userId = 0;
	MovieId bookId = 0;
	int option = 0;
	int bd = 0;

	while(true){
		cout<<endl<<"1) Busqueda"<<endl;
		cout<<"2) Prediccion"<<endl;
		cout<<"Opcion->";
		cin>>option;
		cout<<endl;
		switch(option){
			case 1:{
				cout<<"1) Book"<<endl;
				cout<<"2) Inter"<<endl;
				cout<<"Opcion->";
				cin>>bd;
				cout<<endl;
				switch(bd){
					case 1:{
						cout<<"Id->";
						cin>>bookId;
						printMovie(movieVec[bookId]);
						break;
					}
					case 2:{
						cout<<"Id->";
						cin>>userId;
						printInters(valsByUser[userId]);
						break;
					}
				}
				break;
			}
			case 2:{
				cout<<"UserId->";
				cin>>userId;
				userId--;
				cout<<"BookId->";
				cin>>bookId;
				auto findRes = valsByUser[userId].find(bookId);
				if(findRes != valsByUser[userId].end()){
					cout<<"El libro ya ha sido ranqueado por este usuario"<<endl;
					break;
				}
				auto simVec = getSimilarityVector(valsByProduct, valsByUser, userId, bookId);
				ValType res = prediccionPorProducto(simVec, normVals, userId);
				cout<<"R->"<<res<<endl;
				res = desnormalize(valsByUser, res, userId);
				cout<<"El usuario "<<userId<<" pondría el puntaje "<<res<<" al libro "<<get<0>(movieVec[bookId])<<endl;
			}
		}

	}
}