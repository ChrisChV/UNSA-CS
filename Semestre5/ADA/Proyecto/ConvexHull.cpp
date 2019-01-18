#include <iostream>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <map>
#include "Triangulo.h"

using namespace std;



/*
Punto HallarPuntoMasAlaIzquierda(vector<Punto> &puntos){
	vector<vector<Punto>::iterator> menores;
	for(auto iter = puntos.begin(); iter != puntos.end(); ++iter){
		if(iter == puntos.begin() or (*menores.front()).x > (*iter).x){
			menores.clear();
			menores.push_back(iter);
		}
		else if((*menores.front()).x == (*iter).x){
			menores.push_back(iter);
		}
	}
	sort(menores.begin(), menores.end(),sortIterByY);
	Punto temp = *menores.front();
	puntos.erase(menores.front());
	return temp;
}
*/

void EliminarRepetidos(vector<Punto*> &puntos){
	for(int i = 0; i < puntos.size() - 1; i++){
		if((*puntos[i]) == (*puntos[i+1])){
			cout<<"hola"<<endl;
			puntos.erase(puntos.begin() + i + 1);
			i--;
		}
	}
}

Punto * HallarPuntoMasAlaIzquierda(vector<Punto*> &puntos){
	vector<Punto*>::iterator menor;
	for(auto iter = puntos.begin(); iter != puntos.end(); ++iter){
		if(iter == puntos.begin() or (*menor)->x > (*iter)->x){
			menor = iter;
		}
	}
	vector<Punto *> first;
	first.push_back(*menor);
	for(auto iter = puntos.begin(); iter != puntos.end(); ++iter){
		if((*menor)->x == (*iter)->x) first.push_back(*iter);
	}
	sort(first.begin(), first.end(), sortByY);
	Punto * temp = first.back();
	auto iter = puntos.begin();
	for(; iter != puntos.end(); ++iter){
		if(*iter == temp) break;
	}
	puntos.erase(iter);
	return temp;
}


vector<Triangulo*> generarTriangulos(vector<Punto*> &puntos){
	vector<Triangulo*> res;
	for(int i = 1; i < puntos.size() - 1; i++){
		res.push_back(new Triangulo(puntos[0],puntos[i],puntos[i+1]));
		cout<<"adios"<<endl;
	}
	return res;
}

tuple<vector<Punto*>,vector<Triangulo*>> convexHull(vector<Punto*> puntos){
	Punto * ini = HallarPuntoMasAlaIzquierda(puntos);
	HallarAngulos(puntos,ini);
	sort(puntos.begin(), puntos.end(),sortByAngulo);
	EliminarRepetidos(puntos);
	puntos.insert(puntos.begin(),ini);
	vector<Punto*> res = puntos;
	vector<Triangulo*> triangulos =  generarTriangulos(puntos);
	puntos.insert(puntos.end(),ini);
	//vector<Triangulo> triangulos;
	for(Punto * a : puntos){
		cout<<a->x<<"-"<<a->y<<"->"<<a->angulo<<endl;
	}
	for(int i = 1; i < res.size() - 1; i++){
		if(i == 0) i = 1;
		cout<<"I->"<<i<<endl;
		if(productoVectorial(res[i-1],res[i],res[i+1]) < 0){
			cout<<"A"<<endl;
			triangulos.push_back(new Triangulo(res[i-1],res[i],res[i+1]));		
			cout<<"B"<<endl;
			res.erase(res.begin() + i);
			cout<<"C"<<endl;
			i -= 2;
		}
	}
	return make_tuple(res,triangulos);
}

Uniones generarUniones(vector<Triangulo*> triangulos){
	Uni res;
	for(Triangulo * tri : triangulos)	{
		for(int i = 0; i < 3; i++){
			cout<<"HHHHHHHH"<<endl;
			Punto * a;
			Punto * b;
			if(((*tri)(i))->x < ((*tri)((i+1)%3))->x){
				a = (*tri)(i);
				b = (*tri)((i+1)%3);
			}
			else if (((*tri)(i))->x > ((*tri)((i+1)%3))->x){
				b = (*tri)(i);
				a = (*tri)((i+1)%3);
			}
			else{
				if(((*tri)(i))->y <= ((*tri)((i+1)%3))->y){
					a = (*tri)(i);
					b = (*tri)((i+1)%3);
				}
				else{
					b = (*tri)(i);
					a = (*tri)((i+1)%3);		
				}
			}
			a->imprimirPunto();
			cout<<"->";
			b->imprimirPunto();
			cout<<endl;
			auto tt = make_tuple(a,b);
			auto iter = res.find(tt);
			if(iter == res.end()){
				cout<<"1->";
				(*tri)((i+2)%3)->imprimirPunto();
				cout<<endl;
				res[tt] = new Union(a, b, (*tri)((i+2)%3));
			}
			else{
				cout<<"2->";
				(*tri)((i+2)%3)->imprimirPunto();
				cout<<endl;	
				iter->second->agregarCuarto((*tri)((i+2)%3));
			}
		}
	}	
	Uni result;
	vector<Union*> result2;
	for(auto iter = res.begin(); iter != res.end(); ++iter){
		if(iter->second->hayCuarto()){
			result[iter->first] = iter->second;
			result2.push_back(iter->second);
		} 
	}
	return Uniones(result,result2);
}




int main(){
	srand(time(NULL));

	
	///vector<Punto *> puntos;


	
	vector<Punto*> puntos = generarPuntosAleatorios(500,500,1000);
	ofstream archivo("puntos.txt");
	for(Punto* p : puntos){
		archivo<<"puntos.push_back(new Punto("<<p->x<<","<<p->y<<"));"<<endl;
	}
	auto convex = convexHull(puntos);
	Uniones uniones = generarUniones(get<1>(convex));
	cout<<"MMMMMMMMMMM"<<endl;
	int i = 0;
	//generarGrafica("test2",puntos,get<0>(convex), get<1>(convex));
	//while(!uniones.delaunay){
	//while(i != 2){
		uniones.delaunay = true;
		for(auto iter = uniones.vec_uniones.begin(); iter != uniones.vec_uniones.end(); ++iter){
			Union * u = *iter;
			uniones.verifyFlip(u);
			cout<<"ENDVER"<<endl;
		}
		i++;
		cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<endl;
	//}
	for(auto iter = uniones.begin(); iter != uniones.end(); ++iter){
		iter->second->imprimirUnion();
	}
	cout<<"TT->"<<i<<endl;
	//generarGrafica("test",puntos);
	//generarGrafica("test",puntos,get<0>(convex));
	//generarGrafica("test",puntos,get<0>(convex),get<1>(convex));
	generarGrafica("test2",puntos, get<0>(convex), uniones);
	
	
}