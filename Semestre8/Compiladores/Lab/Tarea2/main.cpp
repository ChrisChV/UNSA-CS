#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

enum Errores {TIPE,ASIG,NOEXVAR,GENERROR};

class Error{
public:
	Error(int a, int b, string s){
		numLinea = a;
		error = b;
		linea = s;
	}
	int numLinea;
	int error;
	string linea;
};
	



int IDfind(vector<string> &v, string s){
	for(int i = 0; i < v.size(); i++){
		if(v[i] == s) return i;
	}
	return -1;
}

bool find(vector<string> &v, string s){
	if(IDfind(v,s) == -1) return false;
	return true;	
}

int main(int argc, char const *argv[]){
	string line = "";
	int estado = 0;
	int numLinea = 0;
	vector<string> idVariables;
	vector<string> tipoVariables;
	try{
		while(true){
			cin>>line;
			numLinea++;
			if(estado == 0 and line == "Inicio"){
				cout<<"#include <iostream>"<<endl<<"using namespace std;"<<endl<<"int main(int argc, char const *argv[]){"<<endl;
				estado = 1;
			} 
			else if(estado == 1 and line == "variables") estado = 2;
			else if(estado == 2){
				string temp = "";
				int num = 0;
				for(char c : line){
					if(c == ',' or c == ':'){
						num++;
						idVariables.push_back(temp);
						temp.clear();
					}
					else temp.push_back(c);
				}
				if(temp != "entero" and temp != "real"){
					throw(Error(numLinea,TIPE,line));
				}
				for(int i = 0; i < num; i++){
					tipoVariables.push_back(temp);
				}
				for(int i = 0; i < idVariables.size(); i++){
					string tipo = "";
					if(tipoVariables[i] == "entero") tipo = "int";
					else if(tipoVariables[i] == "real") tipo = "float";
					cout<<tipo<<" "<<idVariables[i]<<";"<<endl;
				}
				estado = 3;
			}
			else if(estado == 3){
				if(line == "Leer") estado = 4;
				else{
					string temp = "";
					string uno = "";
					string dos = "";
					string tres = "";
					int subestado = 0;
					for(char c : line){
						if(subestado == 0 and c == '<'){
							if(!find(idVariables,temp)) throw(Error(numLinea,NOEXVAR,line));
							uno = temp;
							temp.clear();
							subestado = 1;
						}
						else if(subestado == 1){
							if(c != '-'){
								throw(Error(numLinea,ASIG,line));
							}
							else{
								//temp.push_back(c);
								subestado = 2;	
							}
						}
						else if(subestado == 2 and c == '+'){
							if(!find(idVariables,temp)) throw(Error(numLinea,NOEXVAR,line));
							dos = temp;
							temp.clear();
							subestado = 3;
						}
						else temp.push_back(c);
					}
					if(!find(idVariables,temp)) throw(Error(numLinea,NOEXVAR,line));
					tres = temp;
					cout<<uno<<"="<<dos<<"+"<<tres<<";"<<endl;
					estado = 5;
				}
			}
			else if(estado == 4){
				if(!find(idVariables,line)) throw(Error(numLinea,NOEXVAR,line));
				cout<<"cin>>"<<line<<";"<<endl;
				estado = 3;
			}
			else if(estado == 5 and line == "Escribir")  estado = 6;
			else if(estado == 6){
				if(!find(idVariables,line)) throw(Error(numLinea,NOEXVAR,line));
				cout<<"cout<<"<<line<<"<<endl;"<<endl;
				estado = 7;
			}
			else if(estado == 7){
				cout<<"return 0;"<<endl<<"}"<<endl;
				break;
			}
			else throw(Error(numLinea,GENERROR,line));
		}
	}
	catch(Error e){
		fprintf(stderr,"Line %d:%s\n",e.numLinea,e.linea.c_str());
		switch(e.error){
			case TIPE:
				fprintf(stderr,"No existe el tipo\n");
				break;
			case ASIG:
				fprintf(stderr,"Error en la operacion asignacion\n");
				break;
			case NOEXVAR:
				fprintf(stderr,"No existe la variable\n");
				break;
			case GENERROR:
				fprintf(stderr,"ERROR\n");
				break;
		}
	}
	return 0;
}


