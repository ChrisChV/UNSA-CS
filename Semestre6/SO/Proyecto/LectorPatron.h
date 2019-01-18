#ifndef LECTORPATRON_H
#define LECTORPATRON_H

#include <iostream>
#include <tuple>
#include <fstream>
#include <map>
#include "Step.h"
#include "Mapping.h"

typedef map<string,int> MapColas;
typedef map<int,vector<Step>> MapSteps;
typedef tuple<MapColas,MapSteps> MappingObject;
typedef tuple<int,string> MappingException;

using namespace std;

void printMapColas(MapColas &colas){
	for(auto iter = colas.begin(); iter != colas.end(); ++iter){
		cout<<iter->first<<"->"<<iter->second<<endl;
	}
}

void printMapSteps(MapSteps &steps){
	for(auto iter = steps.begin(); iter != steps.end(); ++iter){
		cout<<iter->first<<": ";
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			cout<<"["<<(*iter2).hardware<<"|"<<(*iter2).bucle<<"|";
			for(auto iter3 = (*iter2).algoritmos.begin(); iter3 != (*iter2).algoritmos.end(); ++iter3){
				cout<<*iter3<<"/";
			}
			cout<<"]->";
		}
		cout<<endl;
	}
}

MappingObject mappingFile(string file){
	ifstream archivo(file.c_str());
	string word;
	string cc;
	MapColas colas;
	vector<Step> steps;
	MapSteps Psteps;
	int estado = 0;
	try{
		while(archivo>>word){
			if(estado == 0 ){
				if(word != "Colas") throw make_tuple(1, word);
				estado = 1;
			}
			else if(estado == 1){
				if(word == "Mapeo") break;
				else{
					cc = word;
					estado = 2;
				}
			}
			else if(estado == 2){
				if(colas.find(cc) != colas.end()) throw make_tuple(3,cc);
				int cod = getAlgorithmCode(word);
				if(cod == -1) throw make_tuple(3,word);
				colas[cc] = cod;
				estado = 1;
			}
		}
		char w[256];
		archivo.getline(w,256);
		while(archivo.getline(w,256)){
			string line(w);
			string prosess = "";
			estado = 0;
			int hw = -1;
			int bucle = -1;
			int prosessCode = -1;
			Step s = Step();
			for(int i = 0; i < line.size(); i++){
				if(estado == 0){
					if(line[i] == '['){
						prosessCode = getProssesCode(prosess);
						if(prosessCode == -1) throw make_tuple(6,prosess);
						if(Psteps.find(prosessCode) != Psteps.end()) throw(7,prosess);
						prosess.clear();
						estado = 1;	
					} 
					else prosess.push_back(line[i]);
				}
				else if(estado == 1){
					if(line[i] != 32){
						if(line[i] == '{'){
							if(bucle != -1) throw make_tuple(4,line);
							bucle = steps.size();
						}
						else if(line[i] == ']'){
							if(steps.size() == 0) throw make_tuple(4,line);
							estado = 4;	
						} 
						else{
							if(hw == -1){
								if(line[i] == 'P') hw = PROCESADOR;
								else if(line[i] == 'E') hw = ES;
								else throw make_tuple(4,line);
							}
							else if(hw == PROCESADOR){
								if(line[i] != 'E') throw make_tuple(4,line);
								hw = ES;
							}
							else if(hw == ES){
								if(line[i] != 'P') throw make_tuple(4,line);
								hw = PROCESADOR;
							}
							estado = 2;
						}
					}
				}
				else if(estado == 2){
					if(line[i] != ':') throw make_tuple(4,line);
					estado = 3;
					cc.clear();
					s.hardware = hw;
				}
				else if(estado == 3){
					if(line[i] == ','){
						s.bucle = -1;
						if(colas.find(cc) == colas.end()) throw make_tuple(5,cc);
						s.algoritmos.push_back(cc);
						steps.push_back(s);
						s = Step();
						estado = 1;
						cc.clear();
					}
					else if(line[i] == '}'){
						if(bucle == -1) throw make_tuple(4,line);
						s.bucle = bucle;
						if(colas.find(cc) == colas.end()) throw make_tuple(5,cc);
						s.algoritmos.push_back(cc);
						steps.push_back(s);
						s = Step();
						estado = 1;
						cc.clear();	
					}
					else if(line[i] == '/'){
						if(colas.find(cc) == colas.end()) throw make_tuple(5,cc);
						s.algoritmos.push_back(cc);
						cc.clear();
					}
					else if(line[i] != 32) cc.push_back(line[i]);
				}
			}
			if(estado != 4) throw make_tuple(4,line);
			Psteps[prosessCode] = steps;
			steps.clear();
		}
		return make_tuple(colas,Psteps);
	}

	catch(string e){
		cout<<"Error en el archivo de mapeo: "<<e<<endl;
		archivo.close();
	}
	catch(MappingException e){
		archivo.close();
		int flag = get<0>(e);
		string s = get<1>(e);
		if(flag == 1) cout<<"La palabra "<<s<<" no concuerda con la sintaxis. Palabra esperada: 'Colas'";
		else if(flag == 2) cout<<"La cola "<<s<<" ya existe"<<endl;
		else if(flag == 3) cout<<"El algoritmo "<<s<<" no existe"<<endl;
		else if(flag == 4) cout<<"La siguiente linea no cumple la sintaxis: "<<s<<endl;
		else if(flag == 5) cout<<"La cola "<<s<<" no existe"<<endl;
		else if(flag == 6) cout<<"El proceso "<<s<<" no existe"<<endl;
		else if(flag == 7) cout<<"El proceso "<<s<<" esta siendo redefinido"<<endl;
	}
	archivo.close();
}



#endif