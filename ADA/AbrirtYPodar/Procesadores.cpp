#include <iostream>
#include <fstream>
#include "NodoTT.h"
#include "Nodo2.h"
#include <map>
#include <queue>

using namespace std;

enum Colores{NEGRO,ROJO,VERDE};

void plot(string file, vector<NodoTT*> arbol){
	ofstream archivo(file);
	archivo<<"digraph{"<<endl;
	for(auto iter = arbol.begin() + 1; iter != arbol.end(); ++iter){
		archivo<<(*iter)->trabajo<<" [label = \""<<(*iter)->trabajo<<" "<<(*iter)->nivel<<"\"""];"<<endl;
		for(auto iter2 = (*iter)->padres.begin(); iter2 != (*iter)->padres.end(); ++iter2){
			archivo<<(*iter2)->trabajo<<"->"<<(*iter)->trabajo<<";"<<endl;
		}
	}
	archivo<<"}";
	archivo.close();
}

int alturamayor(vector<NodoTT*> arbol){
	int mayor = -1;
	for(auto iter = arbol.begin()+1; iter != arbol.end(); ++iter){
		if((*iter)->nivel > mayor) mayor = (*iter)->nivel;
	}
	return mayor;
}

bool sinProcesadores(Nodo * n){
	for(auto iter = n->procesadores.begin(); iter != n->procesadores.end(); ++iter){
		if(*iter != 0) return false;
	}
	return true;
}

void dibujarNodo(Nodo * n, ofstream &archivo, int color){
	string c = "black";
	if(color == 1) c = "red";
	else if(color == 2) c = "green";
	archivo<<n->id<<" [ color = \""<<c<<"\" label = \"";
	for(auto iter = n->prod.begin(); iter != n->prod.end(); ++iter){
		archivo<<*iter<<" ";
	}
	archivo<<"costo="<<n->costo<<"\"];"<<endl;
}

int suma(vector<int> &v){
	int s = 0;
	for(int i = 0; i < v.size(); i++){
		s += v[i];
	}
	return s;
}

int AbrirPodar(Nodo * n, vector<int> &niveles, int cota, int &id, ofstream &archivo, Nodo *& res){
	dibujarNodo(n,archivo,NEGRO);
	if(n->costo >= cota){
		dibujarNodo(n,archivo,ROJO);
		delete n;
		return cota;	
	} 
	if(sinProcesadores(n)){
		dibujarNodo(n,archivo,ROJO);
		delete n;
		return cota;
	}
	if(n->nivel == niveles.size()){
		if(n->costo < cota){
			dibujarNodo(n,archivo,VERDE);
			res = n;
			return n->costo;
		}
		return cota;
	}
	for(int i = 1; i < n->procesadores.size(); i++){
		if(n->procesadores[i] != 0){
			queue<tuple<vector<int>,vector<int>>> anteriores;
			vector<int> r;
			r.push_back(i);
			auto p = n->procesadores;
			p[i] -= 1;
			anteriores.push(make_tuple(r,p));
			while(!anteriores.empty()){
				auto iter = anteriores.front();
				if(suma(get<0>(iter)) >= niveles[n->nivel]){
					int costo = n->costo + get<0>(iter).size();
					id++;
					archivo<<to_string(n->id)<<"->"<<to_string(id)<<";"<<endl;
					cota = AbrirPodar(new Nodo(iter,id,costo,n),niveles,cota,id,archivo,res);
				}
				else{
					for(int j = i; j < n->procesadores.size(); j++){
						vector<int> temp = get<0>(iter);;
						vector<int> temp2 = get<1>(iter);
						if(temp2[j] != 0){		
							temp.push_back(j);
							temp2[j] -= 1;
							anteriores.push(make_tuple(temp,temp2));
						}
					}
				}
				anteriores.pop();
			}
		}
	}
	return cota;
}

vector<NodoTT*> generarTrabajos(string file){
	ifstream archivo(file);
	int s;
	archivo>>s;
	vector<NodoTT*> res(s+1);
	res[0] = nullptr;
	for(int i = 1; i < s+1; i++) res[i] = new NodoTT(i);
	int a;
	int b;
	while(archivo>>a and archivo>>b){
		res[b]->insert(res[a]);
	}
	archivo.close();
	return res;
}

vector<int> generarProcesadores(string file){
	ifstream archivo(file);
	int s;
	vector<int> pro;
	while(archivo>>s){
		pro.push_back(s);
	}
	archivo.close();
	vector<int> res;
	if(pro.size() == 0) return res;
	int mayor = pro[0];
	for(int i = 1; i < pro.size(); i++){
		if(pro[i] > mayor) mayor = pro[i];
	}
	res = vector<int>(mayor+1);
	for(int i = 0; i < pro.size(); i++){
		res[pro[i]] += 1;
	}
	return res;
}

vector<int> levelear(vector<NodoTT*> arbol) {
	vector<int> res(alturamayor(arbol)+1);
	for(auto iter = arbol.begin()+1; iter != arbol.end(); ++iter){
		res[(*iter)->nivel] += 1;
	}
	return res;
}

int main(){
	auto trabajos = generarTrabajos("procesadores.txt");
	auto procesadores = generarProcesadores("procesadores2.txt");
	auto niveles = levelear(trabajos);
	ofstream archivo("eje3.dot");
	archivo<<"digraph{"<<endl;
	vector<int> r;
	r.push_back(0);
	int id = 0;
	int infinito = 10000;
	Nodo * res;
	cout<<AbrirPodar(new Nodo(make_tuple(r,procesadores),0,0,id),niveles,infinito,id,archivo,res)<<endl;
	archivo<<"}";
	archivo.close();
	while(res){
		for(auto iter = res->prod.begin(); iter != res->prod.end(); ++iter){
			cout<<*iter<<" ";
		}
		cout<<endl;
		res = res->padre;
	}
}