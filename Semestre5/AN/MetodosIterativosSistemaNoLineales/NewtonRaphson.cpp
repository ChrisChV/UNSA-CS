#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "OperacionesMatriz.h"
#include "PLU.h"

Num fun1(Lista r){
	//return (8 * r[0] - 4 * pow(r[0],2) + pow(r[1],2) + 1) / 8 - r[0];
	return r[0]*r[0] - r[1]*r[1] - 0.13;
}

Num fun2(Lista r){
	//return (2 * r[0] - pow(r[0],2) + 4 * r[1] - pow(r[1],2) + 3) / 4 - r[1];
	return 2*r[0]*r[1] + 0.76;
}

//or (8*x-4*x*x + y*y+1)/8 - x or (2*x-x*x+4*y-y*y+3)/4 - y

Num der1(Lista r){
	//return 0.125 * (8- 8*r[0]) - 1;
	return 2*r[0];	
}

Num der2(Lista r){
	//return 0.125 * 2 * r[1];
	return -2*r[1];

}

Num der3(Lista r){
	//return 0.25 * (2-2*r[0]);
	return 2*r[1];
}

Num der4(Lista r){
	//return 0.25 * (4 - 2*r[1])-1;
	return 2*r[0];
}

Matriz JF(MatrizFunciones jf, Lista r){
	int n = jf.size();
	Matriz res = zeros(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			res[i][j] = jf[i][j](r);
		}
	}
	return res;
}

Lista F(ListaFunciones f, Lista r){
	Lista res = r;
	for(int i = 0; i < r.size(); i++){
		res[i] = f[i](r);
	}
	return res;
}

Lista F(ListaFunciones f, Lista r, Lista qq){
	Lista res = r;
	for(int i = 0; i < r.size(); i++){
		res[i] =  f[i](r) - qq[i];
	}
	return res;
}

Matriz JF(ListaFunciones f, Lista r, Num h){
	int n = f.size();
	Matriz res = zeros(n);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			Lista temp = r;
			temp[j] += h;
			res[i][j] = (f[i](temp) - f[i](r)) / h;
		}
	}
	return res;
}

void NewtonAprox(ListaFunciones f, Lista r, string fi, Num n, Num presicion, Num h){
	string file = fi + ".csv";
	ofstream archivo(file.c_str());
	archivo.precision(12);
	int i = 0;
	archivo<<"R"<<i<<endl;
	escribirLista(r,archivo);
	Lista r_anterior;
	do{
		r_anterior = r;
		r = r + MetodoPLU(JF(f,r,h),F(f,r) * -1);
		i++;
		archivo<<"R"<<i<<endl;
		escribirLista(r,archivo);
	}while(!ErrorAbsoluto(r_anterior,r,presicion) and i < n);
	archivo<<"Resultado actual"<<endl;
	escribirLista(r,archivo);
	archivo.close();
}

Lista Newton(ListaFunciones f, MatrizFunciones jf, Lista r, string fi, Num n, Num presicion,Lista qq){
	string file = fi + ".csv";
	ofstream archivo(file.c_str());
	archivo.precision(12);
	int i = 0;
	archivo<<"R"<<i<<endl;
	escribirLista(r,archivo);
	Lista r_anterior;
	do{
		r_anterior = r;
		r = r + MetodoPLU(JF(jf,r),F(f,r,qq) * -1);
		i++;
		archivo<<"R"<<i<<endl;
		escribirLista(r,archivo);
	}while(!ErrorAbsoluto(r_anterior,r,presicion) and i < n);
	archivo<<"Resultado actual"<<endl;
	escribirLista(r,archivo);
	archivo.close();
	return r;
}

void Newton2(ListaFunciones f, MatrizFunciones jf, Lista r, string fi, Num n, Num presicion){
	string file = fi + ".csv";
	ofstream archivo(file.c_str());
	archivo.precision(12);
	int i = 0;
	archivo<<"R"<<i<<endl;
	escribirLista(r,archivo);
	Lista r_anterior;
	do{
		r_anterior = r;
		Lista rr = {-1,1};
		
		r = r + Newton(f,jf,rr,"nada",100,0.00000001,MetodoPLU(JF(jf,r),F(f,r) * -1));
		i++;
		archivo<<"R"<<i<<endl;
		escribirLista(r,archivo);
	}while(!ErrorAbsoluto(r_anterior,r,presicion) and i < n);
	archivo<<"Resultado actual"<<endl;
	escribirLista(r,archivo);
	archivo.close();
}

int main(){
	ListaFunciones f;
	Num(*f1)(Lista) = fun1;
	Num(*f2)(Lista) = fun2;
	f.push_back(f1);
	f.push_back(f2);
	ListaFunciones fil1;
	ListaFunciones fil2;
	MatrizFunciones jf;
	Num(*d1)(Lista) = der1;
	Num(*d2)(Lista) = der2;
	Num(*d3)(Lista) = der3;
	Num(*d4)(Lista) = der4;
	fil1.push_back(d1);
	fil1.push_back(d2);
	fil2.push_back(d3);
	fil2.push_back(d4);
	jf.push_back(fil1);
	jf.push_back(fil2);
	Lista r = {-1,1};
	string file;
	Num n;
	Num presicion;
	Num h;
	bool flag;
	cout<<"Ingrese su n->";
	cin>>n;
	cout<<"Ingrese la presicion->";
	cin>>presicion;
	cout<<"Ingrese su h->";
	cin>>h;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	cout<<"Newton Normal(0) - Newton Aproximado (1)->";
	cin>>flag;
	if(flag){
		NewtonAprox(f,r,file,n,presicion,h);
	}
	else Newton2(f,jf,r,file,n,presicion);
	
}