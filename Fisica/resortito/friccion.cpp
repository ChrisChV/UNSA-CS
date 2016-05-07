#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;

#define GRAVEDAD_TIERRA 9.78

typedef long double Number;

enum TT{TIEMPO,POSICION,VELOCIDAD,ACELERACION,FUERZA,ENERGIA_CINETICA,ENERGIA_POTENCIAL,ENERGIA_TOTAL};

typedef tuple<Number,Number,Number,Number,Number,Number,Number,Number> GrupoRes;

void generarExcel(vector<GrupoRes> vec, string fi){
	string file = fi + ".csv";
	ofstream archivo(file);
	archivo<<"\"t\",\"x\",\"v\",\"a\",\"F\",\"Ec\",\"Ey\",\"<E></E>t\""<<endl;
	for(GrupoRes res : vec){
		archivo<<get<TIEMPO>(res)<<","<<get<POSICION>(res)<<","<<get<VELOCIDAD>(res)<<","<<get<ACELERACION>(res)
			<<","<<get<FUERZA>(res)<<","<<get<ENERGIA_CINETICA>(res)<<","<<get<ENERGIA_POTENCIAL>(res)
				<<","<<get<ENERGIA_TOTAL>(res)<<endl;;
	}
	archivo.close();
	cout<<"Archivo generado correctamente"<<endl;
}

void generarGrafica(vector<GrupoRes> vec, string fi){
	string file = fi + ".m";
	ofstream archivo(file);
	archivo<<"#!/usr/bin/octave -qf"<<endl;
	archivo<<"x = [";
	int i = 0;
	for(GrupoRes res : vec){
		if(i != 0) archivo<<",";
		archivo<<get<TIEMPO>(res);
		i++;
	}
	archivo<<"];"<<endl;
	archivo<<"y = [";
	i = 0;
	for(GrupoRes res : vec){
		if(i != 0) archivo<<",";
		archivo<<get<ENERGIA_TOTAL>(res);
		i++;
	}
	archivo<<"];"<<endl;
	archivo<<"plot(x,y,'r*')"<<endl;
	archivo<<"pause()";
	archivo.close();
	string com = "chmod 755 " + file;
	system(com.c_str());
	string com2 = "./" + file;
	system(com2.c_str());
}

float signo(Number x){
	if(x > 0) return 1;
	return -1;
}

vector<GrupoRes> fric(Number tf, Number x0, Number v0, int n, Number m){
	Number t,dt,x,v,a,ww,G,F,k,u,Ec,Ey,Et;
	vector<GrupoRes> res;
	int cN;
	t = 0;
	x = x0;
	v = v0;
	k = 0.5;
	ww = k / m;
	u = 0.6;
	F = 0;
	cN = 0;
	Ec = 0;
	Ey = 0;
	Et = 0,
	G = u * GRAVEDAD_TIERRA;
	dt = tf / n;
	for(int i = 0; i <= n; i++){
		t = i * dt;
		a = -ww * x - G * signo(v); //GRAVEDAD_TIERRA;
		if(i == cN or i == n){
			Ec =  (m * v * v) / 2;
			Ey = (k * x * x) / 2;
			Et = Ec + Ey;
			res.push_back(make_tuple(t,x,v,a,F,Ec,Ey,Et));
			cN += n/500;
		}
		v = v + a * dt;
		x = x + v * dt;
	//W=W+v*v*dt;
	}
	return res;
}

int main(){
	Number tf;
	Number x0;
	Number v0;
	Number m;
	int n;
	string file;
	cout<<"Ingrese su tf->";
	cin>>tf;
	cout<<"Ingrese su x0->";
	cin>>x0;
	cout<<"Ingrese su v0->";
	cin>>v0;
	cout<<"Ingrese su m->";
	cin>>m;
	cout<<"Ingrese su n->";
	cin>>n;
	cout<<"Ingrese el nombre del archivo->";
	cin>>file;
	auto res = fric(tf,x0,v0,n,m);
	generarGrafica(res,file);
	generarExcel(res,file);
}