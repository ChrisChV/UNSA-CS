#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

bool verificar_fun(string & fun, string &res){
	int s  = 1;
	int par = 0;
	int cor = 0;
	int lastcor = 0;
	int lastpar = 0;
	for(auto iter = fun.begin(); iter != fun.end(); ++iter){
		if(*iter != 32 and *iter != 'x' and *iter != '^' and *iter != '+' and *iter != '-' and *iter != '*' and *iter != '/'
			 and (*iter < 48 or *iter > 57)){
			if(*iter == '['){
				if(par != 0){
					res = "NO SE DEBE ABRIR UN CORCHETE SIN CERRAR UN PARENTESIS. col " + to_string(s);
					return false;
				}
				cor++;
				lastcor = s;
			}
			else if(*iter == ']'){
				if(cor == 0){
					res = "EL CORCHETE DE LA col " + to_string(s) + " NO SE A ABIERTO";
					return false;
				}
				if(par != 0){
					res = "ANTES DE CERRAR EL CORCHETE DE LA col " + to_string(s) + "DEBE CERRAR EL PARENTESIS DE LA col " 
					+ to_string(lastpar);
					return false;
				}
				cor--;
				lastcor = s;
			}
			else if(*iter == '('){
				par++;
				lastpar = s;
			}
			else if(*iter == ')'){
				if(par == 0){
					res = "EL PARENTESIS DE LA col " + to_string(s) + " NO SE A ABIERTO";
					return false;
				}
				par--;
				lastpar = s;
			}
			else {
				res = "EL CARACTER " + to_string(*iter) + " DE LA col " + to_string(s) + " NO ES RECONOCIBLE";
				return false;
			}
		}
		s++;
	}
	if(par != 0){
		res = "FALTA CERRAR EL PARENTESIS DE LA col " + to_string(lastpar);
		return false;
	}
	if(cor != 0){
		res = "FALTA CERRAR EL CORCHETE DE LA col " + to_string(lastcor);
		return false;
	}
	res = "FUNCION SINTACTICAMENTE CORRECTA";
	return true;
}

void generar_fun(string & fun, string & name){
	string res;
	if(!verificar_fun(fun, res)){
		cout<<res<<endl;
		return;
	}
	string file = name + ".cpp";
	ofstream archivo(file.c_str());
	archivo<<"#include <iostream>"<<endl;
	archivo<<"#include <cmath>"<<endl<<endl;
	archivo<<"using namespace std;"<<endl<<endl;
	archivo<<"float "<<name<<"(float x){"<<endl;
	archivo<<"return "<<fun<<";"<<endl;
	archivo<<"}"<<endl;
	archivo<<"int main(){"<<endl;
	archivo<<"int x;"<<endl;
	archivo<<"cout<<\"Coloque su x\"<<endl;"<<endl;
	archivo<<"cin>>x;"<<endl;
	archivo<<"cout<<"<<name<<"(x);"<<endl;
	archivo<<"}";
	archivo.close();
}

int main(){
	string fun;
	string name;
	cout<<"Escriba la funcion"<<endl;
	cin>>fun;
	cout<<"Escriba el nombre de la funcion"<<endl;
	cin>>name;
	generar_fun(fun,name);
	string comando = "g++ -std=c++11 " + name + ".cpp -o " + name;
	system(comando.c_str());
	string comando2 = "./" + name;
	system(comando2.c_str());
}