/// XNPIO
/// v 0.0.1

/// Forma Base: <div class="thread">Nombre1, Nombre2<div ....

#include <iostream>
#include <fstream>
#include <map>
#include <tuple>

using namespace std;


void escribirHead(ofstream * archivo){
	ifstream head("head.txt");
	string word;
	while(head>>word) *archivo<<word<<" ";
}

/// Verifica una parte del name2 hasta encontrar un <
int verificarName2(string & word, string & name2){
	for(auto iter = word.begin(); iter != word.end(); ++iter){
		if(*iter == '<') return 1;
		if(*iter == ',') return 2;
		name2.push_back(*iter);
	}
	name2.push_back(' ');
	return 0;
}

/// Verifica una parte del name1 hasta encontrar una coma
int verificarName1(string & word, string & name1){
	name1.push_back(' ');
	for(auto iter = word.begin(); iter != word.end(); ++iter){
		if(*iter == ',') return 1;
		if(*iter == '<') return 2;
		name1.push_back(*iter);
	}
	return 0;
}


/// Verifica si una palabra es la palabra base y saca una parte del primer nombre
int verificarClass(string & word, string & name1){
	auto iter = word.begin();
	string base = "class=\"thread\">";
	for(auto iterBase = base.begin(); iterBase != base.end(); ++iterBase, ++iter) 
		if(*iterBase != *iter) return 0;
	for(; iter != word.end(); iter++){
		if(*iter == ',') return 1;
		if(*iter == '<') return 3;
		name1.push_back(*iter);	
	} 
	return 2;
}

void MessagesAdmin(string & admin){
	map<string,ofstream *> personas;
	ifstream messages("messages.htm");
	string word;
	string name1;
	string name2;
	string noAdmin;
	string mensaje = "";
	string cod = "100008085519060&#064;facebook.com";
	int estado = 0;
	int grupo = 0;
	system("mkdir mensajes");
	try{
		while(messages>>word){
			if(estado == 0){
				int flag = verificarClass(word,name1);
				if(flag == 1){
					estado = 2;
					mensaje += "<div " + word;
				}
				else if(flag == 2){
					estado = 1;
					mensaje += "<div class=\"thread\">";
				}
				else if(flag == 3){
					estado = 3;
					noAdmin = admin;
					auto iter = personas.find(noAdmin);
					if(iter == personas.end()){
						string file =  "mensajes/" + noAdmin + ".html";
						personas[noAdmin] = new ofstream(file.c_str());
						escribirHead(personas[noAdmin]);
					}
					name1.clear();
					name2.clear();
				}
			}
			else if(estado == 1){
				int flag = verificarName1(word, name1);
				if(flag == 1){
					estado = 2;
					mensaje += name1 + ",";
				}
				else if(flag == 2){
					estado = 3;
					noAdmin = admin;
					auto iter = personas.find(noAdmin);
					if(iter == personas.end()){
						string file =  "mensajes/" + noAdmin + ".html";
						personas[noAdmin] = new ofstream(file.c_str());
						escribirHead(personas[noAdmin]);
					}
					name1.clear();
					name2.clear();
				}
			}
			else if(estado == 2){
				int flag = verificarName2(word, name2);
				if(flag == 1){
					estado = 3;
					mensaje += name2 + "<div ";					
					if(admin == name1 or cod == name1) noAdmin = name2;
					else if(admin == name2 or cod == name2) noAdmin = name1;
					else{
						string e = "Proceso Abortado";
						cout<<"No hay admin en la conversación"<<endl;
						cout<<"Persona1->"<<name1<<endl;
						cout<<"Persona2->"<<name2<<endl;
						cout<<"Continuar con el proceso? (s/n)->"<<endl;
						string f;
						cin>>f;
						if(f == "s"){
							noAdmin = name1 + name2;
						}
						else throw e;
						//throw make_tuple(name1,name2);
					}
					cout<<"NOA->"<<noAdmin<<endl;
					auto iter = personas.find(noAdmin);
					if(iter == personas.end()){
						string file =  "mensajes/" + noAdmin + ".html";
						personas[noAdmin] = new ofstream(file.c_str());
						escribirHead(personas[noAdmin]);
					}
					name1.clear();
					name2.clear();
				}
				else if(flag == 2){					
					estado = 3;
					mensaje += name2 + ",";
					grupo++;
					noAdmin = "grupo" + to_string(grupo);
					cout<<"NOA->"<<noAdmin<<endl;
					string file = "mensajes/grupo" + to_string(grupo) + ".html";
					personas[noAdmin] = new ofstream(file.c_str());
					escribirHead(personas[noAdmin]);
					name1.clear();
					name2.clear();
				}
			}
			else if(estado == 3){
				int flag = verificarClass(word,name1);
				if(flag == 1){
					estado = 2;
					mensaje.pop_back();
					mensaje.pop_back();
					mensaje.pop_back();
					mensaje.pop_back();
					mensaje.pop_back();
					*(personas[noAdmin])<<mensaje<<endl;
					mensaje.clear();					
					mensaje += "<div " + word;
				}
				else if(flag == 2){
					estado = 1;
					mensaje.pop_back();
					mensaje.pop_back();
					mensaje.pop_back();
					mensaje.pop_back();
					mensaje.pop_back();
					*(personas[noAdmin])<<mensaje<<endl;
					mensaje.clear();
					mensaje += "<div class=\"thread\">";
				}
				else if(flag == 3){
					estado = 3;
					noAdmin = admin;
					auto iter = personas.find(noAdmin);
					if(iter == personas.end()){
						string file =  "mensajes/" + noAdmin + ".html";
						personas[noAdmin] = new ofstream(file.c_str());
						escribirHead(personas[noAdmin]);
					}
					name1.clear();
					name2.clear();
				}
				else mensaje += word + " ";
			}
		}
		mensaje.pop_back();
		mensaje.pop_back();
		mensaje.pop_back();
		mensaje.pop_back();
		mensaje.pop_back();
		*(personas[noAdmin])<<mensaje<<endl;
		messages.close();
		for(auto iter = personas.begin(); iter != personas.end(); ++iter){
			(*(iter->second)).close();
			delete iter->second;
		}
	}
	catch(string e){cout<<e<<endl;}
	catch(tuple<string,string> tt){
		cout<<"E->"<<get<0>(tt)<<endl;
		cout<<"E->"<<get<1>(tt)<<endl;
	}
}


int main(int argc, char ** argv){
	if(argc == 1){
		cout<<"SIntaxis ./run Admin"<<endl;
		return 0;
	}
	string admin;
	for(int i = 1; i < argc; i++){
		string temp(argv[i]);
		admin += temp + " ";
	}
	admin.pop_back();
	MessagesAdmin(admin);
}