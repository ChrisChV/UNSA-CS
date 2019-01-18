#include <iostream>
#include <fstream>
#include "Nodo.h"
#include "NodoT.h"

using namespace std;

typedef vector<vector<int>> Costos;

bool todasPersonas(Nodo * nodo){
	for(int i = 1; i < nodo->tareas.size(); i++){
		if(nodo->tareas[i] == false) return false;
	}
	return true;
}

bool prereqCompletados(vector<NodoT*>&trabajos, Nodo * nodo, int tarea){
	for(auto iter = trabajos[tarea]->padres.begin(); iter != trabajos[tarea]->padres.end(); ++iter){
		if(!nodo->tareas[(*iter)->trabajo]) return false;
	}
	return true;
}

int AbrirPodar(Nodo * nodo, int cota, ofstream &archivo, vector<NodoT*> &trabajos, int &id, Costos &costos, Nodo *&res){
	if(nodo->costo >= cota){
		archivo<<to_string(nodo->id)<<" [color = red label =\"P"<<to_string(nodo->p)<<" T"<<to_string(nodo->t)<<" "<<to_string(nodo->costo)<<"\"];"<<endl;
		delete nodo;
		return cota;
	}
	else archivo<<to_string(nodo->id)<<" [label = \"P"<<to_string(nodo->p)<<" T"<<to_string(nodo->t)<<" "<<to_string(nodo->costo)<<"\"];"<<endl;
	if(todasPersonas(nodo)){
		if(nodo->costo < cota){
			archivo<<to_string(nodo->id)<<" [color =green];"<<endl;
			res = nodo;
			return nodo->costo;
		} 
		return cota;
	}
	for(int p = 1; p < nodo->personas.size(); p++){
		if(!nodo->personas[p]){
			for(int t = 1; t < nodo->tareas.size();t++){
				if(!nodo->tareas[t] and prereqCompletados(trabajos,nodo,t)){
					int costo = nodo->costo + costos[p][t];
					id++;
					archivo<<to_string(nodo->id)<<"->"<<to_string(id)<<";"<<endl;
					cota = AbrirPodar(new Nodo(nodo->personas,nodo->tareas,p,t,costo,id,nodo),cota,archivo,trabajos,id,costos,res);
				}
			}
		}
	}
	return cota;
}

Costos generarCostos(string file){
	ifstream archivo(file);
	int t;
	archivo>>t;
	Costos res(t+1);
	for(int i = 1; i < t+1; i++){
		res[i] = vector<int>(t+1);
	}
	int i = 1;
	int j = 1;
	int s;
	while(archivo>>s){
		if(i!=t+1){
			res[j][i] = s;
			i++;
		}
		else{
			i = 1;
			j++;
			res[j][i] = s;
			i++;
		}
	}
	return res;
}

vector<NodoT*> generarTrabajos(string file){
	ifstream archivo(file);
	int s;
	archivo>>s;
	vector<NodoT*> res(s+1);
	res[0] = nullptr;
	for(int i = 1; i < s+1; i++) res[i] = new NodoT(i);
	int a;
	int b;
	while(archivo>>a and archivo>>b){
		res[b]->padres.push_back(res[a]);
	}
	return res;
}

int main(){
	Costos costos = generarCostos("costos.txt");
	vector<NodoT*> trabajos = generarTrabajos("trabajos.txt");
	ofstream archivo("eje.dot");
	archivo<<"digraph{"<<endl;
	int id = 0;
	int infinito = 100000;
	Nodo * res;
	int m = AbrirPodar(new Nodo(costos.size()-1,costos.size()-1),infinito,archivo,trabajos,id,costos,res);	
	archivo<<"}";
	cout<<m<<endl;
	while(res){
		cout<<"P"<<res->p<<":T"<<res->t<<endl;
		res = res->padre;
	}
}