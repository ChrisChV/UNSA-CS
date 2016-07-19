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

	/*
	puntos.push_back(new Punto(3,34));
	puntos.push_back(new Punto(45,12));
	puntos.push_back(new Punto(20,65));
	puntos.push_back(new Punto(19,7)); // 5
	puntos.push_back(new Punto(31,54));
	*/

/*
puntos.push_back(new Punto(22,34));
puntos.push_back(new Punto(44,48));
puntos.push_back(new Punto(65,50));
puntos.push_back(new Punto(37,43));
puntos.push_back(new Punto(4,9));
puntos.push_back(new Punto(64,22));
puntos.push_back(new Punto(53,3));
puntos.push_back(new Punto(66,6));
puntos.push_back(new Punto(59,55));
puntos.push_back(new Punto(41,43));
*/


/*
puntos.push_back(new Punto(10,2));
puntos.push_back(new Punto(3,4));
puntos.push_back(new Punto(15,6));
puntos.push_back(new Punto(7,8));
puntos.push_back(new Punto(8,5));
puntos.push_back(new Punto(13,4));
puntos.push_back(new Punto(4,7));
puntos.push_back(new Punto(14,8));
puntos.push_back(new Punto(5,2));
puntos.push_back(new Punto(11,6));
*/

/*
puntos.push_back(new Punto(5,54));
puntos.push_back(new Punto(77,51));
puntos.push_back(new Punto(77,20));
puntos.push_back(new Punto(80,62));
puntos.push_back(new Punto(50,41));
puntos.push_back(new Punto(53,75));
puntos.push_back(new Punto(80,19));
puntos.push_back(new Punto(32,58));
puntos.push_back(new Punto(54,39));
puntos.push_back(new Punto(60,82));
puntos.push_back(new Punto(51,50));
puntos.push_back(new Punto(24,3));
puntos.push_back(new Punto(42,4));
puntos.push_back(new Punto(41,63));
puntos.push_back(new Punto(30,100));
puntos.push_back(new Punto(33,34));
puntos.push_back(new Punto(53,62));
puntos.push_back(new Punto(36,81));
puntos.push_back(new Punto(81,68));
puntos.push_back(new Punto(43,83));
puntos.push_back(new Punto(8,47));
puntos.push_back(new Punto(9,40));
puntos.push_back(new Punto(66,40));
puntos.push_back(new Punto(97,71));
puntos.push_back(new Punto(30,56));
puntos.push_back(new Punto(53,80));
puntos.push_back(new Punto(57,76));
puntos.push_back(new Punto(34,99));
puntos.push_back(new Punto(79,27));
puntos.push_back(new Punto(61,60));
*/

/*
puntos.push_back(new Punto(80,100));
puntos.push_back(new Punto(40,98));
puntos.push_back(new Punto(64,48));
puntos.push_back(new Punto(95,81));
puntos.push_back(new Punto(23,97));
puntos.push_back(new Punto(8,7));
puntos.push_back(new Punto(91,55));
puntos.push_back(new Punto(17,53));
puntos.push_back(new Punto(46,58));
puntos.push_back(new Punto(21,97));
puntos.push_back(new Punto(60,12));
puntos.push_back(new Punto(71,86));
puntos.push_back(new Punto(65,20));
puntos.push_back(new Punto(7,57));
puntos.push_back(new Punto(58,39));
puntos.push_back(new Punto(68,89));
puntos.push_back(new Punto(38,59));
puntos.push_back(new Punto(38,1));
puntos.push_back(new Punto(58,85));
puntos.push_back(new Punto(33,80));
puntos.push_back(new Punto(33,92));
puntos.push_back(new Punto(39,23));
puntos.push_back(new Punto(98,7));
puntos.push_back(new Punto(28,95));
puntos.push_back(new Punto(64,100));
puntos.push_back(new Punto(92,75));
puntos.push_back(new Punto(12,14));
puntos.push_back(new Punto(60,76));
puntos.push_back(new Punto(34,18));
puntos.push_back(new Punto(84,43));
*/

/*
puntos.push_back(new Punto(78,22));
puntos.push_back(new Punto(2,74));
puntos.push_back(new Punto(60,38));
puntos.push_back(new Punto(61,98));
puntos.push_back(new Punto(75,96));
puntos.push_back(new Punto(82,95));
puntos.push_back(new Punto(96,70));
puntos.push_back(new Punto(65,26));
puntos.push_back(new Punto(83,77));
puntos.push_back(new Punto(40,33));
puntos.push_back(new Punto(14,97));
puntos.push_back(new Punto(14,74));
puntos.push_back(new Punto(2,49));
puntos.push_back(new Punto(87,56));
puntos.push_back(new Punto(70,73));
puntos.push_back(new Punto(57,99));
puntos.push_back(new Punto(94,10));
puntos.push_back(new Punto(73,5));
puntos.push_back(new Punto(47,85));
puntos.push_back(new Punto(2,22));
puntos.push_back(new Punto(80,83));
puntos.push_back(new Punto(68,27));
puntos.push_back(new Punto(5,32));
puntos.push_back(new Punto(53,39));
puntos.push_back(new Punto(61,92));
puntos.push_back(new Punto(71,26));
puntos.push_back(new Punto(40,36));
puntos.push_back(new Punto(99,93));
puntos.push_back(new Punto(84,38));
*/

	
	vector<Punto*> puntos = generarPuntosAleatorios(1000,1000,1000);
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
	
	generarGrafica("test2",puntos, get<0>(convex), uniones);
	//generarGrafica("test",puntos,get<0>(convex));
	//generarGrafica("test",puntos);
}