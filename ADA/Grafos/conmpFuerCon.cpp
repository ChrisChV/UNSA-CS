#include <iostream>
#include <algorithm>
#include "Grafo.h"
#include <stack>


using namespace std;

vector<Capsulita> llenartimes(Grafo &g){
	int i = 1;
	vector<Capsulita> res;
	stack<Grafo::Vertice*> v;
	v.push(g.vertices.begin()->second);
	while(!v.empty()){
		auto nodo = v.top();
		v.pop();
		if(nodo->times != 2){
			res.push_back(Capsulita(nodo,i)); push_heap(res.begin(), res.end());
			i++;
			nodo->times++;
			if(nodo->times != 2){
				v.push(nodo);
				for(auto iter = nodo->vecinos.begin(); iter != nodo->vecinos.end(); ++iter){
					if(get<0>(*iter)->times != 2){
						v.push(get<0>(*iter));
					}
				}	
			}
		}
	}
	return res;
}

void print(vector<Capsulita> v){
	int s = v.size();
	for(int i = 0; i < s; i++){
		cout<<v.front().vertice->nombre<<"->"<<v.front().tiempo<<endl;
		pop_heap(v.begin(), v.end()); v.pop_back();
	}
}

void CompFuerConex(Grafo &g){
	auto heap = llenartimes(g);
	print(heap);
	g.transpuesta();
	g.dibujarGrafo("conexo2");
	g.desmarcarGrafo();
	int color = 1;
	int i = 0;
	while(!heap.empty()){
		cout<<i<<endl;
		i++;
		auto nodo = heap.front().vertice;
		pop_heap(heap.begin(), heap.end()); heap.pop_back();
		auto next = nodo;
		if(!nodo->marcado){
			while(true){
				cout<<"N->"<<next->nombre<<endl;
				next->color = color;
				next->marcado = true;
				auto iter = next->vecinos.begin();
				for(iter; iter != next->vecinos.end(); ++iter){
					cout<<"v->"<<get<0>(*iter)->nombre<<endl;
					if(!get<0>(*iter)->marcado or get<0>(*iter) == nodo) break;
				}
				if(get<0>(*iter) == nodo or iter == next->vecinos.end()){
					color++;
					break;
				}
				next = get<0>(*iter);
			}		
		}
	}
	g.transpuesta();
}

int main(){
	/*
	vector<int> a;
	a.push_back(10); push_heap(a.begin(),a.end());
	a.push_back(1); push_heap(a.begin(),a.end());
	a.push_back(120); push_heap(a.begin(),a.end());
	a.push_back(15); push_heap(a.begin(),a.end());
	a.push_back(32); push_heap(a.begin(),a.end());
	int m = a.size();
	for(int i = 0; i < m; i++){
		cout<<a.front()<<endl;
		pop_heap(a.begin(), a.end()); a.pop_back();
	}
	*/
	Grafo g("conexo.txt");
	CompFuerConex(g);
	g.dibujarGrafo("conexo");	
}