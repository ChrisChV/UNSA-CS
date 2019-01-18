#include <iostream>
#include "Grafo.h"

using namespace std;

int _coloring(vector<bool>& colores){
	for(int i = 1; i != colores.size(); i++){
		if(colores[i] == false) return i;
	}
}

void coloring(Grafo &g){
	for(auto iter = g.vertices.begin(); iter != g.vertices.end(); ++iter){
		vector<bool> colores(g.vertices.size() + 1);
		for(auto iter2 = iter->second->vecinos.begin(); iter2 != iter->second->vecinos.end(); ++iter2){
			colores[get<0>(*iter2)->color] = true;
		}
		iter->second->color = _coloring(colores);
	}
}

int main(){
	Grafo g("color.txt");
	coloring(g);
	g.dibujarGrafo("color");
}