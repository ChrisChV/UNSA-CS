#ifndef TRIANGULO_H
#define TRIANGULO_H

#include <iostream>
#include "Punto.h"

using namespace std;

float circuncentro_x(float ax,float ay,float bx,float by ,float cx,float cy){
    float D=2*((ax*(by-cy))+(bx*(cy-ay))+(cx*(ay-by)));
    return (((ay*(pow(cx,2)+pow(cy,2)-pow(bx,2)-pow(by,2)))+(by*(pow(ax,2)+pow(ay,2)-pow(cx,2)-pow(cy,2)))+(cy*(pow(bx,2)+pow(by,2)-pow(ax,2)-pow(ay,2))))/D);
}
float circuncentro_y(float ax,float ay,float bx,float by ,float cx,float cy){
    float D=2*((ax*(by-cy))+(bx*(cy-ay))+(cx*(ay-by)));
    return (((ax*(pow(bx,2)+pow(by,2)-pow(cx,2)-pow(cy,2)))+(cx*(pow(ax,2)+pow(ay,2)-pow(bx,2)-pow(by,2)))+(bx*(pow(cx,2)+pow(cy,2)-pow(ax,2)-pow(ay,2))))/D);
}

void imprimirMatriz(vector<vector<Coordenada>> m){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			cout<<m[i][j]<<" ";
		}
		cout<<endl;
	}
}

Punto * circuncentro(Punto * a ,Punto * b, Punto * c){
	Coordenada D = 2*((a->x*(b->y-c->y)) +(b->x*(c->y-a->y)) +(c->x*(a->y-b->y)) );
 	Coordenada px = ((a->y*(  pow(c->x,2)+pow(c->y,2)-pow(b->x,2)-pow(b->y,2) )) + (b->y*(  pow(a->x,2)+pow(a->y,2)-pow(c->x,2)-pow(c->y,2) )) + (c->x*(  pow(b->x,2)+pow(b->y,2)-pow(a->x,2)-pow(a->y,2) )))/D;
 	Coordenada py = ((a->x*(  pow(b->x,2)+pow(b->y,2)-pow(c->x,2)-pow(c->y,2) )) + (c->x*(  pow(a->x,2)+pow(a->y,2)-pow(b->x,2)-pow(b->y,2) )) + (b->x*(  pow(c->x,2)+pow(c->y,2)-pow(a->x,2)-pow(a->y,2) )))/D;
 	Punto * temp = new Punto(px,py);
 	return temp;
}

long Determinante(vector<vector<Coordenada>> matrix)
{
	cout<<"DET1"<<endl;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
	long res = matrix[0][0]*(matrix[1][1]*(matrix[2][2]*matrix[3][3]-matrix[2][3]*matrix[3][2])
                      -matrix[1][2]*(matrix[2][1]*matrix[3][3]-matrix[2][3]*matrix[3][1])
                      +matrix[1][3]*(matrix[2][1]*matrix[3][2]-matrix[2][2]*matrix[3][1]))
        -matrix[0][1]*(matrix[1][0]*(matrix[2][2]*matrix[3][3]-matrix[2][3]*matrix[3][2])
                      -matrix[1][2]*(matrix[2][0]*matrix[3][3]-matrix[2][3]*matrix[3][0])
                      +matrix[1][3]*(matrix[2][0]*matrix[3][2]-matrix[2][2]*matrix[3][0]))
        +matrix[0][2]*(matrix[1][0]*(matrix[2][1]*matrix[3][3]-matrix[2][3]*matrix[3][1])
                      -matrix[1][1]*(matrix[2][0]*matrix[3][3]-matrix[2][3]*matrix[3][0])
                      +matrix[1][3]*(matrix[2][0]*matrix[3][1]-matrix[2][1]*matrix[3][0]))
        -matrix[0][3]*(matrix[1][0]*(matrix[2][1]*matrix[3][2]-matrix[2][2]*matrix[3][1])
                      -matrix[1][1]*(matrix[2][0]*matrix[3][2]-matrix[2][2]*matrix[3][0])
                      +matrix[1][2]*(matrix[2][0]*matrix[3][1]-matrix[2][1]*matrix[3][0]));
        cout<<"XXXXXXXXXXX->"<<res<<endl;
  return res;
}

vector<vector<Coordenada>> llenarMatriz(vector<Punto*> puntos){
	cout<<"LL1"<<endl;
	vector<vector<Coordenada>> res(4);
	for(int i = 0; i < 4; i++) res[i] = vector<Coordenada>(4);
	res[0][0] = puntos[0]->x;
	res[0][1] = puntos[0]->y;
	res[0][2] = pow(puntos[0]->x,2) + pow(puntos[0]->y,2);
	res[0][3] = 1;
	res[1][0] = puntos[1]->x;
	res[1][1] = puntos[1]->y;
	res[1][2] = pow(puntos[1]->x,2) + pow(puntos[1]->y,2);
	res[1][3] = 1;
	res[2][0] = puntos[2]->x;
	res[2][1] = puntos[2]->y;
	res[2][2] = pow(puntos[2]->x,2) + pow(puntos[2]->y,2);
	res[2][3] = 1;
	res[3][0] = puntos[3]->x;
	res[3][1] = puntos[3]->y;
	res[3][2] =	pow(puntos[3]->x,2) + pow(puntos[3]->y,2);
	res[3][3] = 1;
	cout<<"LL2"<<endl;
	return res;
}


class Union{
	public:
		Union();
		Union(Punto * a, Punto * b);
		Union(Punto * a, Punto * b, Punto * c);
		Union(Punto * a, Punto * b, Punto * c, Punto * d);
		void agregarCuarto(Punto * d);
		void agregarPunto(int i, Punto * a);
		bool hayCuarto();
		void imprimirUnion();
		int buscarPunto(Punto * a);
		Punto * puntos[4];
		Punto * operator()(int i){
			return puntos[i];
		}
		bool visit;
};

Union::Union(){
	puntos[0] = nullptr;
	puntos[1] = nullptr;
	puntos[2] = nullptr;
	puntos[3] = nullptr;
	visit = false;
}

Union::Union(Punto * a, Punto * b){
	puntos[0] = a;
	puntos[1] = b;
	puntos[2] = nullptr;
	puntos[3] = nullptr;
	visit = false;
}

Union::Union(Punto * a, Punto * b, Punto * c){
	puntos[0] = a;
	puntos[1] = b;
	puntos[2] = c;
	puntos[3] = nullptr;
	visit = false;
}

Union::Union(Punto * a, Punto * b, Punto * c, Punto * d){
	puntos[0] = a;
	puntos[1] = b;
	puntos[2] = c;
	puntos[3] = d;
	visit = false;	
}

void Union::agregarPunto(int i, Punto * a){
	puntos[i] = a;
}

int Union::buscarPunto(Punto * a){
	for(int i = 0; i < 4; i++){
		if(puntos[i] == a) return i;
	}
	return -1;
}

void Union::imprimirUnion(){
	for(int i = 0; i < 4; i++){
		puntos[i]->imprimirPunto();
		cout<<"->";
	}
	cout<<endl;
}

void Union::agregarCuarto(Punto * d){
	puntos[3] = d;
}

bool Union::hayCuarto(){
	if(puntos[3]) return true;
	return false;
}


typedef map<tuple<Punto*,Punto*>,Union*> Uni;


class Triangulo{
	public:
		Triangulo();
		Triangulo(Punto * a, Punto * b, Punto * c);
		Punto * puntos[3];
		Punto * operator()(int i){
			return puntos[i];
		}
};

Triangulo::Triangulo(){
	for(int i = 0; i < 3; i++){
		puntos[i] = new Punto();
	}
}

Triangulo::Triangulo(Punto * a, Punto * b, Punto * c){
	puntos[0] = a;
	puntos[1] = b;
	puntos[2] = c;
}




class Uniones{
	public:
		Uniones(){delaunay = false;};
		Uniones(Uni m, vector<Union *> v){
			map_uniones = m;
			vec_uniones = v;
			delaunay = false;
		};
		int size(){
			return vec_uniones.size();
		};
		Union * operator()(int i){
			return vec_uniones[i];
		}
		Uni::iterator begin(){
			return map_uniones.begin();
		}
		Uni::iterator end(){
			return map_uniones.end();
		}
		Uni map_uniones;
		vector<Union *> vec_uniones;
		bool delaunay;
		void _Flip(vector<Union*>::iterator uIter);
		void _verifyFlip(vector<Union*>::iterator uIter);
		void Flip(Union * u);
		void verifyFlip(Union * u);
};

void Uniones::verifyFlip(Union * u){
	cout<<"VERY"<<endl;
	if(u == nullptr) return;
	auto iter = map_uniones.find(make_tuple((*u)(0),(*u)(1)));
	if(iter == map_uniones.end()) return;
	//u->visit = true;
	vector<Punto*> temp;
	temp.push_back((*u)(0));
	temp.push_back((*u)(1));
	temp.push_back((*u)(2));

	sort(temp.begin(), temp.end(), sortByX);
	if(!((*temp[0]) < (*temp[1]))){
		if(!((*temp[1]) < (*temp[2]))){
			sort(temp.begin(), temp.end(), _sortByY);
		}
		else{
			swap(temp[0],temp[1]);
		}
	}
	HallarAngulos(temp, temp.front());
	sort(temp.begin(), temp.end(), sortByAngulo);
	temp.push_back((*u)(3));
	cout<<"P1";
	u->imprimirUnion();
	for(int i = 0; i < 4; i++){
		cout<<"PP1->";
		temp[i]->imprimirPunto();
		cout<<"->";
	}
	cout<<endl;
	long det1 = Determinante(llenarMatriz(temp));
	cout<<"A"<<endl;
	/*
	temp.clear();
	temp.push_back((*u)(0));
	temp.push_back((*u)(1));
	temp.push_back((*u)(3));
	sort(temp.begin(), temp.end(), sortByX);
	HallarAngulos(temp, temp.front());
	sort(temp.begin(), temp.end(), sortByAngulo);
	temp.push_back((*u)(2));
	imprimirMatriz(llenarMatriz(temp));
	long det2 = Determinante(llenarMatriz(temp));
	for(int i = 0; i < 4; i++){
		cout<<"PP2->";
		temp[i]->imprimirPunto();
		cout<<"->";
	}
	cout<<endl;
	cout<<"EEEEEEEE"<<endl;
	u->imprimirUnion();
	cout<<endl<<"D1->"<<det1<<endl;
	cout<<endl<<"D2->"<<det2<<endl;
	*/
	cout<<"def111->"<<det1<<endl;
	if(det1 > 0){
		delaunay = false;
		cout<<"detif"<<endl;
		Flip(u);
	}
	cout<<"GGGG->"<<det1<<endl;
	u->imprimirUnion();
}

void Uniones::Flip(Union * u){
	cout<<"FLIP----------------------------------------->"<<endl;
	Punto * a = (*u)(0);
	Punto * b = (*u)(2);
	Punto * c = (*u)(1);
	Punto * d = (*u)(3);
	auto iter = map_uniones.find(make_tuple(a,c));
	//if(iter != map_uniones.end()) return;


	map_uniones.erase(iter);
	Union * nuevo;

	if((*b) < (*d)){

		nuevo = new Union(b,d,a,c);

		nuevo->imprimirUnion();

		map_uniones[make_tuple(b,d)] = nuevo;
		cout<<vec_uniones.size()<<endl;
		cout<<"RRR"<<endl;
		//(*uIter) = nuevo;
		//vec_uniones.push_back(nuevo);
	} 
	else{
		cout<<"HHHH"<<endl;
		nuevo = new Union(d,b,a,c);	
		map_uniones[make_tuple(d,b)] = nuevo;
		//(*uIter) = nuevo;
		//vec_uniones.push_back(nuevo);
	} 

	nuevo->imprimirUnion();
	//a con d;
	auto tt1 = make_tuple(a,d);
	if(*d < *a) tt1 = make_tuple(d,a);	
	auto iter1 = map_uniones.find(tt1);
	if(iter1 != map_uniones.end()) iter1->second->agregarPunto(iter1->second->buscarPunto(c),b);		
	//a con b;
	auto tt2 = make_tuple(a,b);
	if(*b < *a) tt2 = make_tuple(b,a);
	auto iter2 = map_uniones.find(tt2);
	if(iter2 != map_uniones.end()) iter2->second->agregarPunto(iter2->second->buscarPunto(c),d);
	//d con c;
	auto tt3 = make_tuple(d,c);
	if(*c < *d) tt3 = make_tuple(c,d);
	auto iter3 = map_uniones.find(tt3);
	if(iter3 != map_uniones.end()) iter3->second->agregarPunto(iter3->second->buscarPunto(a),b);
	//c con b;
	auto tt4 = make_tuple(c,b);
	if(*b < *c) tt4 = make_tuple(b,c);
	auto iter4 = map_uniones.find(tt4);
	if(iter4 != map_uniones.end()) iter4->second->agregarPunto(iter4->second->buscarPunto(a),d);

	
	iter1 = map_uniones.find(tt1);
	cout<<"II1"<<endl;
	if(iter1 != map_uniones.end()) {cout<<"a"<<endl; verifyFlip(iter1->second);}
	iter2 = map_uniones.find(tt2);
	cout<<"II2"<<endl;
	if(iter2 != map_uniones.end()) {cout<<"b"<<endl; verifyFlip(iter2->second);}
	iter3 = map_uniones.find(tt3);
	cout<<"II3"<<endl;
	if(iter3 != map_uniones.end()) {cout<<"c"<<endl; verifyFlip(iter3->second);}
	iter4 = map_uniones.find(tt4);
	cout<<"II4"<<endl;
	if(iter4 != map_uniones.end()) {cout<<"d"<<endl; verifyFlip(iter4->second);}
	
	
	cout<<"IIIIII"<<endl;
}


void Uniones::_verifyFlip(vector<Union*>::iterator uIter){
	Union * u = *uIter;
	cout<<"VERY"<<endl;
	if(u == nullptr) return;
	auto iter = map_uniones.find(make_tuple((*u)(0),(*u)(1)));
	if(iter == map_uniones.end()) return;
	//u->visit = true;
	vector<Punto*> temp;
	temp.push_back((*u)(0));
	temp.push_back((*u)(1));
	temp.push_back((*u)(2));

	sort(temp.begin(), temp.end(), sortByX);
	if(!((*temp[0]) < (*temp[1]))){
		if(!((*temp[1]) < (*temp[2]))){
			sort(temp.begin(), temp.end(), _sortByY);
		}
		else{
			swap(temp[0],temp[1]);
		}
	}
	HallarAngulos(temp, temp.front());
	sort(temp.begin(), temp.end(), sortByAngulo);
	temp.push_back((*u)(3));
	cout<<"P1";
	u->imprimirUnion();
	for(int i = 0; i < 4; i++){
		cout<<"PP1->";
		temp[i]->imprimirPunto();
		cout<<"->";
	}
	cout<<endl;
	long det1 = Determinante(llenarMatriz(temp));
	cout<<"A"<<endl;
	/*
	temp.clear();
	temp.push_back((*u)(0));
	temp.push_back((*u)(1));
	temp.push_back((*u)(3));
	sort(temp.begin(), temp.end(), sortByX);
	HallarAngulos(temp, temp.front());
	sort(temp.begin(), temp.end(), sortByAngulo);
	temp.push_back((*u)(2));
	imprimirMatriz(llenarMatriz(temp));
	long det2 = Determinante(llenarMatriz(temp));
	for(int i = 0; i < 4; i++){
		cout<<"PP2->";
		temp[i]->imprimirPunto();
		cout<<"->";
	}
	cout<<endl;
	cout<<"EEEEEEEE"<<endl;
	u->imprimirUnion();
	cout<<endl<<"D1->"<<det1<<endl;
	cout<<endl<<"D2->"<<det2<<endl;
	*/
	cout<<"def111->"<<det1<<endl;
	if(det1 > 0){
		delaunay = false;
		cout<<"detif"<<endl;
		_Flip(uIter);
	}
	cout<<"GGGG->"<<det1<<endl;
	u->imprimirUnion();
}

void Uniones::_Flip(vector<Union*>::iterator uIter){
	Union * u = *uIter;
	cout<<"FLIP----------------------------------------->"<<endl;
	Punto * a = (*u)(0);
	Punto * b = (*u)(2);
	Punto * c = (*u)(1);
	Punto * d = (*u)(3);
	auto iter = map_uniones.find(make_tuple(a,c));
	//if(iter != map_uniones.end()) return;


	map_uniones.erase(iter);
	Union * nuevo;

	if((*b) < (*d)){

		nuevo = new Union(b,d,a,c);

		nuevo->imprimirUnion();

		map_uniones[make_tuple(b,d)] = nuevo;
		cout<<vec_uniones.size()<<endl;
		cout<<"RRR"<<endl;
		(*uIter) = nuevo;
		//vec_uniones.push_back(nuevo);
	} 
	else{
		cout<<"HHHH"<<endl;
		nuevo = new Union(d,b,a,c);	
		map_uniones[make_tuple(d,b)] = nuevo;
		(*uIter) = nuevo;
		//vec_uniones.push_back(nuevo);
	} 

	nuevo->imprimirUnion();
	//a con d;
	auto tt1 = make_tuple(a,d);
	if(*d < *a) tt1 = make_tuple(d,a);	
	auto iter1 = map_uniones.find(tt1);
	if(iter1 != map_uniones.end()) iter1->second->agregarPunto(iter1->second->buscarPunto(c),b);		
	//a con b;
	auto tt2 = make_tuple(a,b);
	if(*b < *a) tt2 = make_tuple(b,a);
	auto iter2 = map_uniones.find(tt2);
	if(iter2 != map_uniones.end()) iter2->second->agregarPunto(iter2->second->buscarPunto(c),d);
	//d con c;
	auto tt3 = make_tuple(d,c);
	if(*c < *d) tt3 = make_tuple(c,d);
	auto iter3 = map_uniones.find(tt3);
	if(iter3 != map_uniones.end()) iter3->second->agregarPunto(iter3->second->buscarPunto(a),b);
	//c con b;
	auto tt4 = make_tuple(c,b);
	if(*b < *c) tt4 = make_tuple(b,c);
	auto iter4 = map_uniones.find(tt4);
	if(iter4 != map_uniones.end()) iter4->second->agregarPunto(iter4->second->buscarPunto(a),d);
	
	
	cout<<"IIIIII"<<endl;
}



void generarGrafica(string fi, vector<Punto*> &all, vector<Punto*> &convex, vector<Triangulo*> &triangulos){
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i]->x);
	}
	archivo<<"];"<<endl;
	archivo<<"y = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i]->y);
	}
	archivo<<"];"<<endl;
	archivo<<"plot(x,y,'*')"<<endl;
	auto iter1 = convex.begin();
	auto iter2 = iter1 + 1;
	for(; iter2 != convex.end(); ++iter1, ++iter2){
		archivo<<"x = ["<<(*iter1)->x<<","<<(*iter2)->x<<"];"<<endl;
		archivo<<"y = ["<<(*iter1)->y<<","<<(*iter2)->y<<"];"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x,y,'r')"<<endl;
	}
	archivo<<"x = ["<<convex.back()->x<<","<<convex.front()->x<<"];"<<endl;
	archivo<<"y = ["<<convex.back()->y<<","<<convex.front()->y<<"];"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x,y,'r')"<<endl;
	for(auto iter = triangulos.begin(); iter != triangulos.end(); ++iter){
		archivo<<"x1 =["<<(*(*iter))(0)->x<<","<<(*(*iter))(1)->x<<"];"<<endl;
		archivo<<"x2 =["<<(*(*iter))(1)->x<<","<<(*(*iter))(2)->x<<"];"<<endl;
		archivo<<"x3 =["<<(*(*iter))(2)->x<<","<<(*(*iter))(0)->x<<"];"<<endl;
		archivo<<"y1 =["<<(*(*iter))(0)->y<<","<<(*(*iter))(1)->y<<"];"<<endl;
		archivo<<"y2 =["<<(*(*iter))(1)->y<<","<<(*(*iter))(2)->y<<"];"<<endl;
		archivo<<"y3 =["<<(*(*iter))(2)->y<<","<<(*(*iter))(0)->y<<"];"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x1,y1,'r')"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x2,y2,'r')"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x3,y3,'r')"<<endl;
	}
	archivo<<"pause()";
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
}

void generarGrafica(string fi, vector<Punto *> &all, vector<Punto *> &convex, Uniones &uniones){
	
	string file = fi + ".dat";
	ofstream archivo(file.c_str());
	
	for(auto iter = all.begin(); iter != all.end(); ++iter){
		archivo<<(*iter)->x<<" "<<(*iter)->y<<endl<<endl;		
	}

	auto iter = convex.begin();
	auto iter2 = convex.begin() + 1;
	for(;iter2 != convex.end(); ++iter,++iter2){
		
	}	

	archivo.close();
	
}

void generarGrafica(string fi, vector<Punto *> &all, vector<Punto *> &convex, Uniones &uniones, bool flag){
	
	string file = fi + ".m";
	ofstream archivo(file.c_str());
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x = [";

	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i]->x);
	}
	archivo<<"];"<<endl;
	archivo<<"y = [";
	for(int i = 0; i < all.size(); i++){
		if(i != 0) archivo<<",";
		archivo<<to_string(all[i]->y);
	}
	archivo<<"];"<<endl;
	archivo<<"plot(x,y,'*')"<<endl;
	auto iter1 = convex.begin();
	auto iter2 = iter1 + 1;
	for(; iter2 != convex.end(); ++iter1, ++iter2){
		archivo<<"x = ["<<(*iter1)->x<<","<<(*iter2)->x<<"];"<<endl;
		archivo<<"y = ["<<(*iter1)->y<<","<<(*iter2)->y<<"];"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x,y,'r')"<<endl;
	}
	archivo<<"x = ["<<convex.back()->x<<","<<convex.front()->x<<"];"<<endl;
	archivo<<"y = ["<<convex.back()->y<<","<<convex.front()->y<<"];"<<endl;
	archivo<<"hold on"<<endl;
	archivo<<"plot(x,y,'r')"<<endl;

	for(auto iter3 = uniones.begin(); iter3 != uniones.end(); ++iter3){
		archivo<<"x = ["<<(*(iter3->second))(0)->x<<","<<(*(iter3->second))(1)->x<<"];"<<endl;
		archivo<<"y = ["<<(*(iter3->second))(0)->y<<","<<(*(iter3->second))(1)->y<<"];"<<endl;
		archivo<<"hold on"<<endl;
		archivo<<"plot(x,y,'r')"<<endl;
	}

	if(flag){
		for(auto iter3 = uniones.begin(); iter3 != uniones.end(); ++iter3){
			Union * u = iter3->second;
			Punto * a = (*u)(0);
			Punto * b = (*u)(2);
			Punto * c = (*u)(1);
			Punto * d = (*u)(3);
			int centro1X = circuncentro_x(a->x,a->y,c->x,c->y,b->x,b->y);
			int centro1Y = circuncentro_y(a->x,a->y,c->x,c->y,b->x,b->y);
			cout<<"centro->"<<centro1X<<"->"<<centro1Y<<endl;
			//Punto * centro2 = circuncentro(a,c,d);
			Coordenada radio1 = sqrt(pow(a->x - centro1X,2) + pow(a->y - centro1Y,2));
			//Coordenada radio2 = sqrt(pow(a->x - centro2->x,2) + pow(a->y - centro2->y,2));
			archivo<<"hold on"<<endl;
			archivo<<"plot ("<<centro1X<<","<<centro1Y<<")"<<endl;
			//archivo<<"hold on"<<endl;
			//archivo<<"plot ("<<centro2->x<<","<<centro2->y<<")"<<endl;
			string x = "x = [";
			string y = "y = [";
			for(float  i = 0; i<= 30; i+=0.001){
				x += to_string(radio1 * cos(i) + centro1X) + ",";
				y += to_string(radio1 * sin(i) + centro1Y) + ",";
			}
			x.pop_back();
			y.pop_back();

			archivo<<x<<"];"<<endl;
			archivo<<y<<"];"<<endl;
			archivo<<"hold on"<<endl;
			archivo<<"plot(x,y)"<<endl;
			/*
			archivo<<"x = "<<centro1X - radio1<<":0.1:"<<centro1X + radio1<<";"<<endl;
			archivo<<"y = sqrt("<<pow(radio1,2)<<"-x.^2);"<<endl;
			archivo<<"hold on"<<endl;
			archivo<<"plot(x,y,x,-y)"<<endl;
			archivo<<"x = "<<centro2X - radio2<<":0.1:"<<centro2X + radio2<<";"<<endl;
			archivo<<"y = sqrt("<<pow(radio2,2)<<"-x.^2);"<<endl;
			archivo<<"hold on"<<endl;
			archivo<<"plot(x,y,x,-y)"<<endl;
			*/
		}
	}

	archivo<<"pause()";
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
}


#endif