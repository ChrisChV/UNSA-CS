#include <iostream>
#include <vector>
#include "Punto.h"
#include <tuple>
#include <algorithm>

using namespace std;

typedef tuple<vector<Punto>::iterator,vector<Punto>::iterator> Tangente ;



float _Distancia(int x1, int y1, int x2, int y2){
	return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}

float Distancia(tuple<Punto,Punto> tt){
	Punto p1 = get<0>(tt);
	Punto p2 = get<1>(tt);
	return sqrt(pow(p2.x - p1.x,2) + pow(p2.y - p1.y,2));
}

bool _sortByY(tuple<Punto,int> a, tuple<Punto,int> b){
	return (get<0>(a).y < get<0>(b).y);
}

bool sortByDistancia(tuple<Punto,Punto,int,int> a,tuple<Punto,Punto,int,int> b){
	return (Distancia(make_tuple(get<0>(a),get<1>(a))) < Distancia(make_tuple(get<0>(b),get<1>(b))));
}

void generarGrafica(vector<Punto> &all, vector<Punto> &convex,string fi){
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i].x);
	}
	archivo<<"];"<<endl;
	archivo<<"y = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i].y);
	}
	archivo<<"];"<<endl;
	archivo<<"plot(x,y,'*')"<<endl;
	auto iter1 = convex.begin();
	auto iter2 = iter1 + 1;
	for(; iter2 != convex.end(); ++iter1, ++iter2){
		archivo<<"x = ["<<(*iter1).x<<","<<(*iter2).x<<"];"<<endl;
		archivo<<"y = ["<<(*iter1).y<<","<<(*iter2).y<<"];"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x,y,'r')"<<endl;
	}
	archivo<<"x = ["<<convex.back().x<<","<<convex.front().x<<"];"<<endl;
	archivo<<"y = ["<<convex.back().y<<","<<convex.front().y<<"];"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x,y,'r')"<<endl;
	archivo<<"pause()";
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
}

vector<int> mayorX(vector<Punto> &izquierda){
	int mayor = izquierda[0].x;
	vector<int> res;
	res.push_back(0);
	for(int i = 1; i < izquierda.size(); i++){
		if(mayor == izquierda[i].x){
			res.push_back(i);
		}
		else if(mayor < izquierda[i].x){
			res.clear();
			res.push_back(i);
			mayor = izquierda[i].x;
		}
	}
	return res;
}

vector<int> menorX(vector<Punto> &derecha){
	int menor = derecha[0].x;
	vector<int> res;
	res.push_back(0);
	for(int i = 1; i < derecha.size(); i++){
		if(menor == derecha[i].x){
			res.push_back(i);
		}
		else if(menor > derecha[i].x){
			res.clear();
			res.push_back(i);
			menor = derecha[i].x;
		}
	}
	return res;	
}

bool _tangenteInferior(Punto a, Punto b, Punto c, Punto d){
	//float pendiente = x/y;
	float d1 = (b.y - a.y) * d.x + (a.x-b.x)*d.y + (b.x*a.y-b.y*a.x);
	float d2 = (b.y - a.y) * c.x + (a.x-b.x)*c.y + (b.x*a.y-b.y*a.x);
	//if(pendiente >= 0){
		if(d1 <= 0 and d2 <= 0) return true;
	//}
	//else if(d1 >= 0 and d2 >= 0) return true;
	return false;
}

bool _tangenteSuperior(Punto a, Punto b, Punto c, Punto d){
	//float pendiente = x/y;
	float d1 = (b.y - a.y) * d.x + (a.x-b.x)*d.y + (b.x*a.y-b.y*a.x);
	float d2 = (b.y - a.y) * c.x + (a.x-b.x)*c.y + (b.x*a.y-b.y*a.x);
	//if(pendiente >= 0){
		if(d1 >= 0 and d2 >= 0) return true;
	//}
	//else if(d1 <= 0 and d2 <= 0) return true;
	return false;
}

Tangente tangenteInferior(vector<Punto> &izquierda, vector<Punto> &derecha, int izq, int der){
	vector<Punto>::iterator iterIzq = izquierda.begin() + izq;
	vector<Punto>::iterator iterDer = derecha.begin() + der;
	vector<Punto>::iterator iterIzqAntH; 
	vector<Punto>::iterator iterIzqH;
	vector<Punto>::iterator	iterDerAntH;
	vector<Punto>::iterator	iterDerH;
	if(iterIzq == izquierda.begin()) iterIzqAntH = izquierda.end() - 1;
	else iterIzqAntH = iterIzq - 1;
	if(iterIzq == izquierda.end() - 1) iterIzqH = izquierda.begin();
	else iterIzqH = iterIzq + 1;

	if(iterDer == derecha.begin()) iterDerAntH = derecha.end() - 1;
	else iterDerAntH = iterDer - 1;
	if(iterDer == derecha.end() - 1) iterDerH = derecha.begin();
	else iterDerH = iterDer + 1;
	while(!_tangenteInferior(*iterIzq,*iterDer,*iterIzqH,*iterIzqAntH) 
			or !_tangenteInferior(*iterIzq,*iterDer,*iterDerH,*iterDerAntH)){
		//cout<<"AA"<<endl;
		while(!_tangenteInferior(*iterIzq,*iterDer,*iterIzqH,*iterIzqAntH)){
			//cout<<"A"<<endl;
			if(iterIzq == izquierda.end() - 1) iterIzq =izquierda.begin();
			else iterIzq++;
			if(iterIzq == izquierda.begin()) iterIzqAntH = izquierda.end() - 1;
			else iterIzqAntH = iterIzq - 1;
			if(iterIzq == izquierda.end() - 1) iterIzqH = izquierda.begin();
			else iterIzqH = iterIzq + 1;
		}
		while(!_tangenteInferior(*iterIzq,*iterDer,*iterDerH,*iterDerAntH)){
			//cout<<"B"<<endl;
			
			/*
			cout<<"Ix->"<<(*iterIzq).x<<endl;
			cout<<"Iy->"<<(*iterIzq).y<<endl;
			cout<<"Dx->"<<(*iterDer).x<<endl;
			cout<<"Dy->"<<(*iterDer).y<<endl;
			cout<<"Dhx->"<<(*iterDerH).x<<endl;
			cout<<"Dhy->"<<(*iterDerH).y<<endl;
			cout<<"Dahx->"<<(*iterDerAntH).x<<endl;
			cout<<"Dahy->"<<(*iterDerAntH).y<<endl;
			*/
			//cout<<"Inicio->"<<(*derecha.begin()).x<<endl;	
			if(iterDer == derecha.begin()) iterDer = derecha.end() - 1;
			else iterDer--;
			if(iterDer == derecha.begin()) iterDerAntH = derecha.end() - 1;
			else iterDerAntH = iterDer - 1;
			if(iterDer == derecha.end() - 1) iterDerH = derecha.begin();
			else iterDerH = iterDer + 1;
		}
	}
	return make_tuple(iterIzq,iterDer);
}

Tangente tangenteSuperior(vector<Punto> &izquierda, vector<Punto> &derecha, int izq, int der){
	vector<Punto>::iterator iterIzq = izquierda.begin() + izq;
	vector<Punto>::iterator iterDer = derecha.begin() + der;
	vector<Punto>::iterator iterIzqAntH; 
	vector<Punto>::iterator iterIzqH;
	vector<Punto>::iterator	iterDerAntH;
	vector<Punto>::iterator	iterDerH;
	if(iterIzq == izquierda.begin()) iterIzqAntH = izquierda.end() - 1;
	else iterIzqAntH = iterIzq - 1;
	if(iterIzq == izquierda.end() - 1) iterIzqH = izquierda.begin();
	else iterIzqH = iterIzq + 1;

	if(iterDer == derecha.begin()) iterDerAntH = derecha.end() - 1;
	else iterDerAntH = iterDer - 1;
	if(iterDer == derecha.end() -1) iterDerH = derecha.begin();
	else iterDerH = iterDer + 1;
	while(!_tangenteSuperior(*iterIzq,*iterDer,*iterIzqH,*iterIzqAntH) 
			or !_tangenteSuperior(*iterIzq,*iterDer,*iterDerH,*iterDerAntH)){
		//cout<<"CCC"<<endl;
		while(!_tangenteSuperior(*iterIzq,*iterDer,*iterIzqH,*iterIzqAntH)){
			//cout<<"C"<<endl;
			if(iterIzq == izquierda.begin()) iterIzq =izquierda.end() - 1;
			else iterIzq--;
			if(iterIzq == izquierda.begin()) iterIzqAntH = izquierda.end() - 1;
			else iterIzqAntH = iterIzq - 1;
			if(iterIzq == izquierda.end() - 1) iterIzqH = izquierda.begin();
			else iterIzqH = iterIzq + 1;
		}
		while(!_tangenteSuperior(*iterIzq,*iterDer,*iterDerH,*iterDerAntH)){
			//cout<<"CC"<<endl;
			/*
			cout<<"Ix->"<<(*iterIzq).x<<endl;
			cout<<"Iy->"<<(*iterIzq).y<<endl;
			cout<<"Dx->"<<(*iterDer).x<<endl;
			cout<<"Dy->"<<(*iterDer).y<<endl;
			cout<<"Dhx->"<<(*iterDerH).x<<endl;
			cout<<"Dhy->"<<(*iterDerH).y<<endl;
			cout<<"Dahx->"<<(*iterDerAntH).x<<endl;
			cout<<"Dahy->"<<(*iterDerAntH).y<<endl;
			*/
			if(iterDer == derecha.end() - 1) iterDer = derecha.begin();
			else iterDer++;
			if(iterDer == derecha.begin()) iterDerAntH = derecha.end() - 1;
			else iterDerAntH = iterDer - 1;
			if(iterDer == derecha.end() - 1) iterDerH = derecha.begin();
			else iterDerH = iterDer + 1;
		}
	}
	return make_tuple(iterIzq,iterDer);
}

vector<Punto> mergeSort(vector<Punto> izquierda, vector<Punto> 	derecha){
	vector<Punto> res;
	vector<int> vecIzq = mayorX(izquierda);
	vector<int> vecDer = menorX(derecha);
	int izq;
	int der;
	vector<tuple<Punto,int>> _vecIzq;
	vector<tuple<Punto,int>> _vecDer;
	for(int i : vecIzq){
		_vecIzq.push_back(make_tuple(izquierda[i],i));
	}
	for(int i : vecDer){
		_vecDer.push_back(make_tuple(derecha[i],i));	
	}
	
	sort(_vecIzq.begin(), _vecIzq.end(),_sortByY);
	sort(_vecDer.begin(), _vecDer.end(),_sortByY);
	vecIzq.clear();
	vecDer.clear();
	for(tuple<Punto,int> t : _vecIzq){
		vecIzq.push_back(get<1>(t));
	}
	for(tuple<Punto,int> t : _vecDer){
		vecDer.push_back(get<1>(t));
	}
	if(vecIzq.size() == 1 and vecDer.size() == 1){
		izq = vecIzq.front();
		der = vecDer.front();
	}
	else if(vecIzq.size() == 1){
		izq = vecIzq.front();
		if(Distancia(make_tuple(izquierda[izq],derecha[vecDer.front()])) 
			< Distancia(make_tuple(izquierda[izq],derecha[vecDer.back()]))){
			der = vecDer.front();
		}
		else der = vecDer.back();
	}
	else if(vecDer.size() == 1){
		cout<<"FFFFFFFFFFFFFFFFFFFFFFF"<<endl;
		der = vecDer.front();
		cout<<"DERDERx->"<<derecha[der].x<<endl;
		cout<<"DERDERy->"<<derecha[der].y<<endl;
		if(Distancia(make_tuple(izquierda[vecIzq.front()],derecha[der]))
			< Distancia(make_tuple(izquierda[vecIzq.back()],derecha[der]))){
			izq = vecIzq.front();
		}
		else izq = vecIzq.back();
	}
	else{
		vector<tuple<Punto,Punto,int,int>> dis;
		dis.push_back(make_tuple(izquierda[vecIzq.front()],derecha[vecDer.front()],vecIzq.front(),vecDer.front()));
		dis.push_back(make_tuple(izquierda[vecIzq.front()],derecha[vecDer.back()],vecIzq.front(),vecDer.back()));
		dis.push_back(make_tuple(izquierda[vecIzq.back()],derecha[vecDer.front()],vecIzq.back(),vecDer.front()));
		dis.push_back(make_tuple(izquierda[vecIzq.back()],derecha[vecDer.back()],vecIzq.back(),vecDer.back()));
		sort(dis.begin(), dis.end(),sortByDistancia);
		izq = get<2>(dis.front());
		der = get<3>(dis.front());
	}
	cout<<"Fizqx1->"<<izquierda[izq].x<<endl;
	cout<<"Fizqy1>"<<izquierda[izq].y<<endl;
	cout<<"Fizqx2->"<<derecha[der].x<<endl;
	cout<<"Fizqy2->"<<derecha[der].y<<endl;


	Tangente tanInf = tangenteInferior(izquierda,derecha,izq,der);
	Tangente tanSup = tangenteSuperior(izquierda,derecha,izq,der);
	cout<<"TanInfx1->"<<(*(get<0>(tanInf))).x<<endl;
	cout<<"TanInfy1->"<<(*(get<0>(tanInf))).y<<endl;
	cout<<"TanInfx2->"<<(*(get<1>(tanInf))).x<<endl;
	cout<<"TanInfy2->"<<(*(get<1>(tanInf))).y<<endl;
	cout<<"TanSup1x->"<<(*(get<0>(tanSup))).x<<endl;
	cout<<"TanSup1y->"<<(*(get<0>(tanSup))).y<<endl;
	cout<<"TanSup2x->"<<(*(get<1>(tanSup))).x<<endl;
	cout<<"TanSup2y->"<<(*(get<1>(tanSup))).y<<endl;
	vector<Punto>::iterator iter = get<0>(tanInf);
	
	/*
	for(auto iter = izquierda.begin(); iter != get<0>(tanSup) + 1; ++iter){
		cout<<"ITER1x->"<<(*iter).x<<endl;
		cout<<"ITER1y->"<<(*iter).y<<endl;
		res.push_back(*iter);
	}
	
	
	for(auto iter = get<1>(tanSup); iter != get<1>(tanInf) + 1; ++iter){
		cout<<"ITER2x->"<<(*iter).x<<endl;
		cout<<"ITER2y->"<<(*iter).y<<endl;
		res.push_back(*iter);
	}
	
	
	
	for(auto iter = get<0>(tanInf); iter != izquierda.end() and iter != izquierda.begin(); ++iter){
		cout<<"ITER3x->"<<(*iter).x<<endl;
		cout<<"ITER3y->"<<(*iter).y<<endl;
		res.push_back(*iter);
	}
	*/
	cout<<"AAAAAAAAAAAAAAAAAA"<<endl;
	for(int i = 0; i < izquierda.size();i++){
		cout<<i<<"->x->"<<izquierda[i].x<<",y->"<<izquierda[i].y<<endl;
	}
	while(iter != get<0>(tanSup)){
		cout<<"IterX1->"<<(*iter).x<<endl;
		cout<<"IterY1->"<<(*iter).y<<endl;
		res.push_back(*iter);
		if(iter == izquierda.end() - 1) iter = izquierda.begin();
		else iter++;
	}
	res.push_back(*iter);
	cout<<"IterX2->"<<(*iter).x<<endl;
	cout<<"IterY2->"<<(*iter).y<<endl;
	iter = get<1>(tanSup);
	while(iter != get<1>(tanInf)){
		cout<<"IterX3->"<<(*iter).x<<endl;
		cout<<"IterY3->"<<(*iter).y<<endl;
		res.push_back(*iter);
		if(iter == derecha.end() - 1) iter = derecha.begin();
		else iter++;
	}
	res.push_back(*iter);
	cout<<"IterX4->"<<(*iter).x<<endl;
	cout<<"IterY4->"<<(*iter).y<<endl;
	return res;
}

void ordenar(vector<Punto> &vec){
	if(vec.size() == 1) return;
	sort(vec.begin(), vec.end(),sortByX);
	int ini = 0;
	for(int i = 1; i < vec.size(); i++){
		if(vec[ini].x != vec[i].x){
			if(i - ini != 1) sort(vec.begin() + ini, vec.begin() + i,sortByY);
			ini = i;
		}
	}
	sort(vec.begin() + ini, vec.end(),sortByY);
}

vector<Punto> convexHull(vector<Punto> &vec, int ini, int fin){
	vector<Punto> res;
	if(fin - ini == 1){
		if(vec[ini].y > vec[fin].y){
			res.push_back(vec[ini]);
			res.push_back(vec[fin]);	
		}
		else{
			res.push_back(vec[fin]);
			res.push_back(vec[ini]);	
		}
		return res;
	}
	if(fin - ini == 2){
		if(vec[ini].x == vec[ini + 1].x){
			if(vec[ini +1].x == vec[fin].x){
				res.push_back(vec[fin]);
				res.push_back(vec[ini]);			
				res.push_back(vec[ini +1]);
				sort(res.begin(), res.end(),sortByY);
				return res;
			}
			res.push_back(vec[ini]);
			res.push_back(vec[ini+1]);
			sort(res.begin(), res.end(),sortByY);
			res.push_back(vec[fin]);
			return res;
		}
		res.push_back(vec[ini]);
		if(vec[ini +1].x == vec[fin].x){
			if(vec[ini+1].y > vec[fin].y){
				res.push_back(vec[ini+1]);
				res.push_back(vec[fin]);
				return res;
			}
			res.push_back(vec[fin]);
			res.push_back(vec[ini+1]);
			return res;
		}
		if(vec[ini].y == vec[fin].y){
			if(vec[ini+1].y >= vec[fin].y){
				res.push_back(vec[ini+1]);
				res.push_back(vec[fin]);
				return res;
			}
			res.push_back(vec[fin]);
			res.push_back(vec[ini+1]);
		}
		if(vec[ini + 1].y > vec[fin].y){
			res.push_back(vec[ini+1]);
			res.push_back(vec[fin]);
			return res;
		}
		res.push_back(vec[fin]);
		res.push_back(vec[ini+1]);
		return res;
	}
	int M = (ini + fin) / 2;
	vector<Punto> a = convexHull(vec,ini,M);
	vector<Punto> b = convexHull(vec,M+1,fin);
	cout<<"Izquierda"<<endl;
	for(int i = 0; i < a.size(); i++){
		cout<<i<<"->x->"<<a[i].x<<",y->"<<a[i].y<<endl;
	}
	cout<<endl;
	//generarGrafica(vec,a,"a.m");
	cout<<"Derecha"<<endl;
	for(int i = 0; i < b.size(); i++){
		cout<<i<<"->x->"<<b[i].x<<",y->"<<b[i].y<<endl;
	}
	cout<<endl;
	//generarGrafica(vec,b,"b.m");
	return mergeSort(a,b);
}

int main(){
	srand (time(NULL));
	int xMax;
	int yMax;
	int numPuntos;
	string file;
	cout<<"Ingresa su xMax->";
	cin>>xMax;
	cout<<"Ingrese su yMax->";
	cin>>yMax;
	cout<<"Ingrese su numero maximo de puntos->";
	cin>>numPuntos;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	vector<Punto> vec = generarPuntosAleatorios(xMax,yMax,numPuntos);

//vector<Punto>vec;
	

	ofstream archivo("puntos.txt");
	ordenar(vec);
	for(Punto p : vec){
		archivo<<"vec.push_back(Punto("<<p.x<<","<<p.y<<"));"<<endl;
	}
	vector<Punto> res = convexHull(vec,0,vec.size() - 1);
	cout<<"lo hizo"<<endl;
	

	generarGrafica(vec,res,file);
}