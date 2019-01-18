#include <iostream>
#include <stack>
#include "Grafo.h"

using namespace std;

int costoMin(vector<Grafo::Vertice*> &camino, Grafo &g){
	int min = 10101;
	auto iter1 = camino.begin();
	for(auto iter2 = camino.begin()+1; iter2 != camino.end(); ++iter1,++iter2){
		int temp = g.buscarArista((*iter1)->nombre,(*iter2)->nombre)->costo;
		if(temp < min) min = temp;
	}
	return min;
}

Grafo flujoMaximo(Grafo &g, string a, string b){
	Grafo res(g.file);
	res.quitarCostos();
	auto tempA = g.vertices.find(a);
	auto tempB = g.vertices.find(b);
	if(tempA == g.vertices.end() or tempB == g.vertices.end()) return res;
	auto nodoA = tempA->second;
	auto nodoB = tempB->second;
	stack<Grafo::Vertice*> pila;
	vector<Grafo::Vertice*> camino;
	pila.push(nodoA);
	while(!pila.empty()){
		auto actual = pila.top();
		cout<<"N->"<<actual->nombre<<endl;
		if(actual->marcado){
			cout<<"1"<<endl;
			camino.pop_back();
			pila.pop();
			actual->marcado = false;
		}
		else if(actual == nodoB){
			cout<<"2"<<endl;
			camino.push_back(actual);
			int min = costoMin(camino, g);
			auto iter1 = camino.begin();
			for(auto iter2 = camino.begin()+1; iter2 != camino.end(); ++iter1,++iter2){
				g.buscarArista((*iter1)->nombre,(*iter2)->nombre)->costo -= min;
				res.buscarArista((*iter1)->nombre,(*iter2)->nombre)->costo += min;
			}
			camino.pop_back();
			pila.pop();
		}
		else{
			cout<<"3"<<endl;
			actual->marcado = true;
			camino.push_back(actual);
			for(auto iter = actual->vecinos.begin(); iter != actual->vecinos.end(); ++iter){
				cout<<"NN->"<<get<0>(*iter)->nombre<<endl;
				if(!get<0>(*iter)->marcado and get<1>(*iter) > 0) pila.push(get<0>(*iter));
			}
		}
	}
	return res;
}

int main(){
	Grafo g("flujo.txt");
	auto gg = flujoMaximo(g,"1","4");
	gg.dibujarGrafo("flujo");
}