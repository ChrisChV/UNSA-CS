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


typedef int UserId;
typedef string UserCity;
typedef string UserAge;
typedef tuple<UserCity, UserAge> UserRegister;
typedef string BookId;
typedef string BookName;
typedef string BookAge;
typedef string BookEditorial;
typedef string BookImage;
typedef tuple<BookName, BookAge, BookEditorial, BookImage, BookImage, BookImage> BookRegister;
typedef ValType Valoration;
typedef tuple<BookId, Valoration> InterRegister;

typedef map<BookId, Valoration> InterUserRegisterMap;

MyTime mytime;

void printUser(UserRegister user){
	cout<<get<0>(user)<<" "<<get<1>(user)<<endl;
}

void printInters(InterUserRegisterMap interMap){
	for(auto iter = interMap.begin(); iter != interMap.end(); ++iter){
		cout<<iter->first<<" "<<iter->second<<endl;
	}
}

void printBook(BookRegister book){
	cout<<get<0>(book)<<" "<<get<1>(book)<<" "<<get<2>(book)<<" "<<get<3>(book)<<" "<<get<4>(book)<<" "<<get<5>(book)<<endl;
}

int main(){
	cout<<"Cargando Base de Datos..."<<endl;
	mytime.init();
	auto bdUsers = getBd("BX-Dump/BX-Users.csv", ';');
	auto bdInter = getBd("BX-Dump/BX-Book-Ratings.csv", ';');
	auto bdBooks = getBd("BX-Dump/BX-Books.csv", ';');
	mytime.end();
	cout<<"Done->";
	mytime.print();

	UserId id = 0;

	cout<<"Generando estructura User..."<<endl;
	mytime.init();
	
	vector<UserRegister> userVec(bdUsers.size() + 1);
	for(auto iter = bdUsers.begin(); iter != bdUsers.end(); ++iter){
		id = stoi((*iter)[0]);
		userVec[id] = make_tuple((*iter)[1],(*iter)[2]);
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();

	cout<<"Generando estructura Book..."<<endl;
	mytime.init();
	map<BookId, BookRegister> bookMap;
	for(auto iter = bdBooks.begin(); iter != bdBooks.end(); ++iter){
		bookMap[(*iter)[0]] = make_tuple((*iter)[1],(*iter)[2],(*iter)[3],(*iter)[4],(*iter)[5],(*iter)[6]);
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();

	cout<<"Generando estructura InterByUser..."<<endl;
	mytime.init();
	vector<InterUserRegisterMap> valsByUser(bdUsers.size() + 1);
	Valoration val = 0;	
	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		id = stoi((*iter)[0]);
		val = stoi((*iter)[2]);
		/*Valor implícito 0*/
		//if(val == 0) continue;
		valsByUser[id][(*iter)[1]] = val;
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();


	cout<<"Generando estructura InterByProduct..."<<endl;
	mytime.init();
	map<BookId, map<UserId,ValType>> valsByProduct;
	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		val = stoi((*iter)[2]);
		id = stoi((*iter)[0]);
		/*Valor implicito 0 */
		//if(val == 0) continue;

		/*
		auto findRes = valsByProduct.find((*iter)[1]);
		if(findRes == valsByProduct.end()){
			valsByProduct[(*iter)[1]] = vector<ValType>(bdUsers.size() + 1, NULL_VAL);
		}
		*/

		valsByProduct[(*iter)[1]][id] = val;
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();

	bdUsers.clear();
	bdInter.clear();
	bdBooks.clear();

	cout<<"Normalizando Datos..."<<endl;
	mytime.init();
	auto normVals = normalizate(valsByUser);
	mytime.end();
	cout<<"Done->";
	mytime.print();	

	UserId userId = 0;
	BookId bookId = "";
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
				cout<<"1) User"<<endl;
				cout<<"2) Book"<<endl;
				cout<<"3) Inter"<<endl;
				cout<<"Opcion->";
				cin>>bd;
				cout<<endl;
				switch(bd){
					case 1:{
						cout<<"Id->";
						cin>>userId;
						printUser(userVec[userId]);
						break;
					}
					case 2:{
						cout<<"Id->";
						cin>>bookId;
						printBook(bookMap[bookId]);
						break;
					}
					case 3:{
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
				cout<<"El usuario "<<userId<<" pondría el puntaje "<<res<<" al libro "<<get<0>(bookMap[bookId])<<endl;
			}
		}

	}
}