#include <iostream>
#include <fstream>
#include <cmath>
#include "formulas.h"

using namespace std;

// x-x^(x-cosx)  = 0
// x - cos(sen(x)) = 0
// x^5 - 3x^3 - 2x^2 + 2 = x

enum Tipos{SECANTE = 1, NEWTON_A, NEWTON_B};

Number funcion(Number x){
	//return x - pow(x,x-cos(x));
	//return x - cos(sin(x));
	//return pow(x,5) - 3 * pow(x,3) - 2 * x * x + 2 - x;
	//return pow(x,3) - 1.265 * x + 1;
	return pow(x,3) - 9 * pow(x,2) + 27 * x - 27;
}

Number _funcion(Number x){
	//return 1 - pow(x,x-cos(x)) * (((x-cos(x)) / x) + log(x) * (sin(x) + 1));
	//return sin(sin(x)) * cos(x) + 1;
	//return 5 * pow(x,4) - 9 * x * x - 4 * x - 1;
	//return 3 * x * x - 1.265;
	return 3 * pow(x,2) - 18 * x + 27;
}

Number _MetodoSecante(Number r0, Number r1, Number fr0, Number fr1){
	return r0-((r1-r0)/(fr1-fr0))*fr0;
}

Number _MetodoNewton_A(Number r, Number fr, Number f_r){
	return r - (fr/f_r);
}

Number _MetodoNewton_B(Number r, Number fr, Number frh, Number h){
	return r - ((fr * h)/(frh - fr));
}

void MetodoSecante(Number r0, Number r1, Number(*f)(Number), int n, Number presicion, string fi){
	string file = fi + ".csv";
	ofstream archivo(file);
	Number fr0 = f(r0);
	Number fr1 = f(r1);
	Number fr2 = 0;
	Number r2 = 0;
	int i = 1;
	archivo<<"\"i\",\"ri-1\",\"ri\",\"f(ri-1)\",\"f(ri)\",\"ri+1\",\"f(r1+1)\""<<endl;
	do{
		r2 = _MetodoSecante(r0,r1,fr0,fr1);
		fr2 = f(r2);
		archivo<<i<<","<<r0<<","<<r1<<","<<fr0<<","<<fr1<<","<<r2<<","<<fr2<<endl;
		r0 = r1;
		r1 = r2;
		fr0 = fr1;
		fr1 = fr2;
		i++;
	}while((ErrorAbsoluto(r0,r1) > presicion and i != n) or i == 2);
	archivo<<"\"Resultado Final\","<<r1<<endl;
	archivo.close();
}


void MetodoNewton_A(Number r, Number(*f)(Number), Number(*df)(Number), Number presicion, int n, string fi){
	string file = fi + ".csv";
	ofstream archivo(file);
	Number fr = 0;
	Number f_r = 0;
	Number r2 = 0;
	Number r_anteriror;
	int i = 0;
	archivo<<"\"i\",\"ri\",\"f(ri)\",\"f'(r1)\",\"ri+1\""<<endl;
	do{
		r_anteriror = r;
		fr = f(r);
		f_r = df(r);
		r2 = _MetodoNewton_A(r,fr,f_r);
		archivo<<i<<","<<r<<","<<fr<<","<<f_r<<","<<r2<<endl;
		r = r2;
		i++;
	}while((ErrorAbsoluto(r_anteriror, r) > presicion and i != n) or i == 1);
	archivo<<"\"Resultado actual\","<<r<<endl;
	archivo.close();
}

void MetodoNewton_B(Number r, Number(*f)(Number), Number h, Number presicion, int n, string fi){
	string file = fi + ".csv";
	ofstream archivo(file);
	Number fr = 0;
	Number frh = 0;
	Number r2 = 0;
	Number r_anteriror = 0;
	int i = 0;
	archivo<<"\"i\",\"ri\",\"f(ri)\",\"f(ri+h)\",\"ri+1\""<<endl;
	do{
		r_anteriror = r;
		fr = f(r);
		frh = f(r+h);
		r2 = _MetodoNewton_B(r,fr,frh,h);
		archivo<<i<<","<<r<<","<<fr<<","<<frh<<","<<r2<<endl;
		r = r2;
		i++;
	}while((ErrorAbsoluto(r_anteriror, r) > presicion and i != n) or i == 1);
	archivo<<"\"Resultado actual\","<<r<<endl;
	archivo.close();
}

int main(){
	Number r0;
	int n;
	Number presicion;
	Number (*f)(Number) = funcion;
	string file;
	int tipo;
	cout<<"Que metodo quiere usar->Secante(1) - Newton_A (2) - Newton_B (3)";
	cin>>tipo;
	cout<<"Ingrese el r0->";
	cin>>r0;
	cout<<"Ingrese el la presicion->";
	cin>>presicion;
	cout<<"Ingrese n->";
	cin>>n;
	cout<<"Ingrese nombre del archivo dende saldra la tabla->";
	cin>>file;
	if(tipo == SECANTE){
		Number r1;
		cout<<"Ingrese el r1->";
		cin>>r1;
		MetodoSecante(r0,r1,f,n,presicion,file);
		cout<<"Archivo generado correctamente"<<endl;
	}
	else if(tipo == NEWTON_A){
		Number (*df)(Number) = _funcion;
		MetodoNewton_A(r0,f,df,presicion,n,file);
		cout<<"Archivo generado correctamente"<<endl;
	}
	else if(tipo == NEWTON_B){
		Number h;
		cout<<"Ingrese el h->";
		cin>>h;
		MetodoNewton_B(r0,f,h,presicion,n,file);
		cout<<"Archivo generado correctamente"<<endl;
	}
	else cout<<"Ocurrio algo"<<endl;
}