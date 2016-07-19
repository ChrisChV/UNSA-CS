#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include "colores.h"

using namespace std;

enum dir{SIN_DIR,A_B,B_A};
enum tipo{GRAPH,DIGRAPH};

class Grafo{
	public:
		class Vertice{
			public:
				Vertice(){
				};
				Vertice(string nombre){
					this->nombre = nombre;
					this->color = 0;
					this->marcado = false;
					this->times = 0;
				};
				void insertarVecino(Vertice * ver, int costo){
					vecinos.push_back(make_tuple(ver,costo));
				};
				string nombre;
				bool marcado;
				int color;
				int times;
				vector<tuple<Vertice*,int>> vecinos;
		};
		class Arista{
			public:
				Arista(){
					costo = 0;
					direccion = SIN_DIR;
					vertices[0] = nullptr;
					vertices[1] = nullptr;
				}
				Arista(int costo, int direccion, Vertice * a, Vertice * b){
					this->costo = costo;
					this->direccion = direccion;
					vertices[0] = a;
					vertices[1] = b;
				};
				Vertice * getVecino(Vertice * a){
					if(vertices[0] == a) return vertices[1];
					else vertices[0];
				}
				Vertice * operator()(int i){
					if(i != 0 and i != 1) return nullptr;
					return vertices[i];
				}
				Vertice * vertices[2];
				int costo;
				int direccion;
		};
		Grafo(){
			tipe = GRAPH;
		};
		Grafo(string file){
			generarGrafo(file);
			this->file = file;
		};
		bool tipe;
		string file;
		map<string,Vertice *> vertices;
		vector<Arista *> aristas;
		Vertice * crearVertice(string nombre);
		Arista * agregarArista(Vertice * a, Vertice * b, int costo, int direccion);
		Arista * agregarArista(string a, string b, int costo, int direccion);
		bool generarGrafo(string file);
		bool dibujarGrafo(string fi);
		void desmarcarGrafo();
		void transpuesta();
		void _transpuesta(Vertice * actual, Vertice * anterior);
		void quitarCostos();
		Arista * buscarArista(string a, string b);
};

class Capsulita{
	public:
		Capsulita(){
			vertice = nullptr;
			tiempo = 0;
		};
		Capsulita(Grafo::Vertice * vertice, int tiempo){
			this->vertice = vertice;
			this->tiempo = tiempo;
		}
		bool operator ==(Capsulita b){
			return (this->tiempo == b.tiempo);
		}
		bool operator <(Capsulita b){
			return (this->tiempo < b.tiempo);	
		}
		bool operator <=(Capsulita b){
			return (this->tiempo <= b.tiempo);	
		}
		bool operator >(Capsulita b){
			return (this->tiempo > b.tiempo);	
		}
		bool operator >=(Capsulita b){
			return (this->tiempo >= b.tiempo);	
		}
		bool operator !=(Capsulita b){
			return (this->tiempo != b.tiempo);	
		}
		Grafo::Vertice * vertice;
		int tiempo;
};

typename Grafo::Vertice * Grafo::crearVertice(string nombre){
	Vertice * nuevo = new Vertice(nombre);
	vertices[nombre] = nuevo;
	return nuevo;
}

typename Grafo::Arista * Grafo::agregarArista(Grafo::Vertice * a, Grafo::Vertice * b, int costo, int direccion){
	Arista * nuevo = new Arista(costo,direccion,a,b);
	if(direccion == SIN_DIR){
		a->insertarVecino(b,costo);
		b->insertarVecino(a,costo);	
	}
	else if(direccion == A_B){
		a->insertarVecino(b,costo);
	}
	else if(direccion == B_A){
		b->insertarVecino(a,costo);
	}
	aristas.push_back(nuevo);
	return nuevo;
}

typename Grafo::Arista * Grafo::agregarArista(string sa, string sb, int costo, int direccion){
	auto iter1 = vertices.find(sa);
	auto iter2 = vertices.find(sb);
	if(iter1 == vertices.end() or iter2 == vertices.end()) return nullptr;
	Vertice * a = iter1->second;
	Vertice * b = iter2->second;
	Arista * nuevo = new Arista(costo,direccion,a,b);
	if(direccion == SIN_DIR){
		a->insertarVecino(b,costo);
		b->insertarVecino(a,costo);	
	}
	else if(direccion == A_B){
		a->insertarVecino(b,costo);
	}
	else if(direccion == B_A){
		b->insertarVecino(a,costo);
	}
	aristas.push_back(nuevo);
	return nuevo;
}


bool Grafo::generarGrafo(string file){
	ifstream archivo(file);
	string a;
	string b;
	string d;
	int c;
	archivo>>a;
	if(a != "{") return false;
	archivo>>a;
	while(a != "}"){
		crearVertice(a);
		archivo>>a;
	}
	archivo>>a;
	if(a == "d") tipe = DIGRAPH;
	else if(a == "g") tipe = GRAPH;
	else return false;
	while(archivo>>a and archivo>>d and archivo>>b and archivo>>c){
		int dir = 0;
		if(d == "-" or d == "<->") dir = SIN_DIR;
		else if(d == "->") dir = A_B;
		else if(d == "<-") dir = B_A;
		else return false;
		if(agregarArista(a,b,c,dir) == nullptr) return false;
	}
	return true;
}

bool Grafo::dibujarGrafo(string fi){
	string file = fi + ".dot";
	ofstream archivo(file);
	if(tipe == GRAPH) archivo<<"graph{"<<endl;
	else archivo<<"digraph{"<<endl;
	for(auto iter = vertices.begin(); iter != vertices.end(); ++iter){
		archivo<<iter->second->nombre<<" [color="<<getColor(iter->second->color);
		if(iter->second->color != 0) archivo<<",style=filled";
		archivo<<"];"<<endl;
	}
	for(auto iter = aristas.begin(); iter != aristas.end(); ++iter){
		if((*iter)->direccion == SIN_DIR){
			archivo<<(**iter)(0)->nombre<<" -- "<<(**iter)(1)->nombre;
		}
		else if((*iter)->direccion == A_B){
			archivo<<(**iter)(0)->nombre<<"->"<<(**iter)(1)->nombre;
		}
		else if((*iter)->direccion == B_A){
			archivo<<(**iter)(1)->nombre<<"->"<<(**iter)(0)->nombre;	
		}
		archivo<<" [label="<<(*iter)->costo<<"];"<<endl;
	}
	archivo<<"}";
	archivo.close();
	string com = "dot -Tpdf " + file +" -o " + fi +  ".pdf";
	system(com.c_str());
	return true;
}

void Grafo::desmarcarGrafo(){
	for(auto iter = vertices.begin(); iter != vertices.end(); ++iter){
		iter->second->marcado = false;
	}
}

void Grafo::_transpuesta(Grafo::Vertice * actual, Grafo::Vertice * anterior){
	if(actual->marcado) return;
	actual->marcado = true;
	cout<<"NN->"<<actual->nombre<<endl;
	if(anterior != nullptr) cout<<"A->"<<anterior->nombre<<endl;		
	int s = actual->vecinos.size();
	for(int i = 0; i < s; i++){
		auto t = actual->vecinos[i];
		Vertice * temp = get<0>(t);
		int costo = get<1>(t);
		cout<<"T->"<<temp->nombre<<endl;		
		if(temp != anterior){
			cout<<"T2->"<<temp->nombre<<endl;		
			actual->vecinos.erase(actual->vecinos.begin() + i);
			i--;
			s--;
			temp->vecinos.push_back(make_tuple(actual,costo));
			_transpuesta(temp,actual);
		}
	}
}

void Grafo::transpuesta(){
	for(auto iter = aristas.begin(); iter != aristas.end(); ++iter){
		if((*iter)->direccion == A_B) (*iter)->direccion = B_A;
		else if((*iter)->direccion == B_A) (*iter)->direccion = A_B;
	}
	desmarcarGrafo();
	for(auto iter = vertices.begin(); iter != vertices.end(); ++iter){
		_transpuesta(iter->second,nullptr);
	}
}

void Grafo::quitarCostos(){
	for(auto iter = aristas.begin(); iter != aristas.end(); ++iter){
		(*iter)->costo = 0;
	}
}

typename Grafo::Arista * Grafo::buscarArista(string a, string b){
	for(Arista * arista : aristas){
		if((*arista)(0)->nombre == a and (*arista)(1)->nombre == b and arista->direccion == A_B) return arista;
		if((*arista)(1)->nombre == a and (*arista)(0)->nombre == b and arista->direccion == B_A) return arista;
	}
}

#endif