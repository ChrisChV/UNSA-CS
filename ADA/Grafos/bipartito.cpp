#include <iostream>
#include "Grafo.h"

using namespace std;

int _coloring(vector<bool>& colores){
	for(int i = 1; i != colores.size(); i++){
		if(colores[i] == false) return i;
	}
	return 1000;
}

bool bipartito(Grafo &g){
	for(auto iter = g.vertices.begin(); iter != g.vertices.end(); ++iter){
		vector<bool> colores(3);
		for(auto iter2 = iter->second->vecinos.begin(); iter2 != iter->second->vecinos.end(); ++iter2){
			colores[get<0>(*iter2)->color] = true;
		}
		int temp = _coloring(colores) ;
		if(temp == 1000) return false;
		iter->second->color = temp;
	}
	return true;
}

int main(){
	Grafo g("bi.txt");
	cout<<bipartito(g)<<endl;
	g.dibujarGrafo("bi");
}