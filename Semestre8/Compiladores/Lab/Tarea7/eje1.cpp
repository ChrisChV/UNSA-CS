#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

char preanalisis = 0;
int contadorCarac = 0;
string cadena = "";
vector<int> errores;
vector<char> sugerencia;



void error(){
	if(errores.empty()) return;
	cout<<cadena<<endl;
	int anterior = -1;
	int actual = 0;
	for(int e : errores){
		actual = e;
		if(anterior == -1){
			anterior = 0;
			actual++;
		}
		for(int i = 0; i < actual - anterior - 1; i++){
			cout<<" ";
		}
		cout<<"^";
		anterior = e;
		//exit(0);	
	}
	cout<<endl;
	for(int i = 0; i < errores.size(); i++){
		cout<<"Error de sintaxis en la posicion "<<errores[i] + 1<<" ,caracter: "<<cadena[errores[i]]<<" Se sugiere el caracter: "<<sugerencia[i]<<endl;
	}
	
}

void parea(char t){
	if(preanalisis == t){
		if(preanalisis != 'c'){
			preanalisis = getchar();
			cadena.push_back(preanalisis);
			contadorCarac++;		
		}
	} 
	else{
		errores.push_back(contadorCarac - 1);
		sugerencia.push_back(t);
	} 
}

void B(){
	if(preanalisis == 'b') parea('b');
	else{
		errores.push_back(contadorCarac - 1);
		preanalisis = getchar();
		contadorCarac++;
		cadena.push_back(preanalisis);
		if(preanalisis == 'b'){
			sugerencia.push_back('a');
			B();
			parea('c');
		} 
		else{
			sugerencia.push_back('b');
			parea('c');
		} 
	}
}

void A(){
	if(preanalisis == 'a') parea('a');
	else{
		errores.push_back(contadorCarac - 1);
		preanalisis = getchar();
		contadorCarac++;
		cadena.push_back(preanalisis);
		if(preanalisis == 'a'){
			A();
			B();
			parea('c');
		} 
		else{
			sugerencia.push_back('a');
			B();
			parea('c');
		} 
	}
}



void S(){
	if(preanalisis == 'x'){
		parea('x');
		S();
	}
	else if(preanalisis == 'a'){
		A();
		B();
		parea('c');
	}
	else{
		errores.push_back(contadorCarac - 1);
		preanalisis = getchar();
		contadorCarac++;
		cadena.push_back(preanalisis);
		if(preanalisis == 'a'){
			sugerencia.push_back('x');
			A();
			B();
			parea('c');
		}
		else if(preanalisis == 'b'){
			sugerencia.push_back('a');
			B();
			parea('c');
		} 
		else {
			sugerencia.push_back('x');
			S();
		}
		
	} 
}


int main(int argc, char **argv)
{
	preanalisis = getchar();
	cadena.push_back(preanalisis);
	contadorCarac++;
	S();
	error();
	
	return 0;
}

