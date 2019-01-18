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
	
	auto bdMovies = getBd("../ml/ml-1m/movies.dat", ':');
	auto bdInter = getBd("../ml/ml-1m/ratings.dat", ';');
	
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

	int numUsers = 6040;

	vector<InterRegisterMap> valsByUser(numUsers);
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
	double vm;
	double rss;
	process_mem_usage(vm,rss);
	cout<<"VM: "<<vm<<" RSS: "<<rss<<endl;


	while(true){
		cout<<endl<<"1) Busqueda"<<endl;
		cout<<"2) Prediccion"<<endl;
		cout<<"3) Nuevo Usuario"<<endl;
		cout<<"4) Iniciar Sesion"<<endl;
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
						userId--;
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
				cout<<"MovieId->";
				cin>>bookId;
				auto findRes = valsByUser[userId].find(bookId);
				if(findRes != valsByUser[userId].end()){
					cout<<"La pelicula ya ha sido ranqueado por este usuario"<<endl;
					break;
				}
				auto simVec = getSimilarityVector(valsByProduct, valsByUser, userId, bookId);
				cout<<"Calculando predicción..."<<endl;
				mytime.init();
				ValType res = prediccionPorProducto(simVec, normVals, userId);
				cout<<"R->"<<res<<endl;
				if(res == INFINITO_NEGATIVO) res = 0;
				else res = desnormalize(valsByUser, res, userId);
				mytime.end();
				cout<<"Done->";
				mytime.print();
				cout<<"El usuario "<<userId + 1<<" pondría el puntaje "<<res<<" al la pelicula "<<get<0>(movieVec[bookId])<<endl;
				break;
			}
			case 3:{
				numUsers++;
				valsByUser.push_back(InterRegisterMap());
				cout<<"Nuevo usuario creado"<<endl;
				cout<<"Id del nuevo usuario->"<<numUsers<<endl;
				break;
			}
			case 4:{
				cout<<"UserId->";
				cin>>userId;
				userId--;
				cout<<"Bienvenido usuario "<<userId + 1<<endl;
				bool flag = true;
				Valoration val = 0;
				while(flag){
					cout<<"1) Mis ranqueados"<<endl;
					cout<<"2) Ranquear pelicula"<<endl;
					cout<<"3) Cerrar Sesion"<<endl;	
					cout<<"Opcion->";
					cin>>option;
					cout<<endl;
					switch(option){
						case 1:{
							printInters(valsByUser[userId]);
							break;
						}
						case 2:{
							cout<<"MovieId->";
							cin>>bookId;
							auto findRes = valsByUser[userId].find(bookId);
							if(findRes != valsByUser[userId].end()){
								cout<<"La pelicula ya ha sido ranqueada por este usuario"<<endl;
								cout<<"Quiere cambiar el puntaje?? 1) Si 2) No"<<endl;
								cout<<"Opcion->";
								cin>>option;
								if(option == 2) break;
							}
							cout<<"Puntaje->";
							cin>>val;
							valsByUser[userId][bookId] = val;
							valsByProduct[bookId][userId] = val;
							cout<<"Pelicula ranqueada correctamente"<<endl<<endl;
							break;
						}
						case 3:{
							flag = false;
							cout<<"Normalizando Datos..."<<endl;
							mytime.init();
							normVals = normalizate(valsByUser);
							mytime.end();
							cout<<"Done->";
							mytime.print();	
							break;
						}
					}
				}
				break;

			}
		}

	}
}