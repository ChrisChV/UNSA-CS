#ifndef NODO_H
#define NODO_H

#include <iostream>
#include <vector>

using namespace std;

class Nodo{
	public:
		Nodo();
		Nodo(int,int);
		Nodo(vector<bool>,vector<bool>,int,int,int,int,Nodo*);
		~Nodo();
		vector<bool> personas;
		vector<bool> tareas;
		Nodo * padre;
		int costo;
		int id;
		int p;
		int t;
};

Nodo::Nodo(){
	padre = nullptr;
	costo = 0;
}

Nodo::Nodo(int per, int tar){
	personas = vector<bool>(per+1);
	tareas = vector<bool>(tar+1);
	padre = nullptr;
	costo = 0;
	id = 0;
	p = 0;
	t = 0;
}

Nodo::Nodo(vector<bool> a, vector<bool> b,int per, int tar, int cost, int id, Nodo* p){
	personas = a;
	personas[per] = true;
	tareas = b;
	tareas[tar] = true;
	costo = cost;
	this->id = id;
	padre = p;
	this->p = per;
	this->t = tar;
}

Nodo::~Nodo(){
}

#endif