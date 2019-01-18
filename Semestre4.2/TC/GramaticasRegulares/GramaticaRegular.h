#ifndef GRAMATICAREGULAR_H
#define GRAMATICAREGULAR_H
#include <fstream>
#include <iostream>
#include <map>
#include "fstream"

using namespace std;

class GramaticaRegular
{
    public:
        GramaticaRegular();
        GramaticaRegular(string file);
        void imprimirTabla();
        void generarAutomata();
        void reconocerCadena(string);
    protected:
    private:
     map<char,map<char,char>> tablaDeTransiciones;
     char estadoInicial;
     bool _reconocerCadena(string cadena);
};

bool GramaticaRegular::_reconocerCadena(string cadena){
    char estadoActual = estadoInicial;
    for(auto iter = cadena.begin(); iter != cadena.end(); ++iter){
        if(estadoActual == '#') return false;
        if(tablaDeTransiciones.begin()->second.find(*iter) != tablaDeTransiciones.begin()->second.end()){
            estadoActual = tablaDeTransiciones[estadoActual][*iter];
        }
        else return false;
    }
    if(tablaDeTransiciones[estadoActual]['%'] == '$')return true;
    return false;
}

void GramaticaRegular::reconocerCadena(string file){
    ifstream archivo(file.c_str());
    if(archivo.fail()){
	cout<<"El archivo "<<file<< "no se puede abrir o no existe"<<endl; 
	return;
    }
    char cad[128];
    while(archivo.getline(cad,128)){
    	string cadena(cad);
	if(_reconocerCadena(cadena)){
       	     cout<<"El automata reconoce la cadena "<<cadena<<endl;
	}
	else cout<<"El automata no reconoce la cadena"<<cadena<<endl;
    }
}

void GramaticaRegular::generarAutomata(){
    ofstream archivo("eje.dot");
    char es = estadoInicial;
    archivo<<"digraph{"<<endl;
    for(auto iter = tablaDeTransiciones.begin(); iter != tablaDeTransiciones.end(); ++iter){
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
            if(iter2->first != '%'){
                if(iter2->second != '#'){
                    archivo<<iter->first<<"->"<<iter2->second<<"[label =\""<<iter2->first<<"\"];"<<endl;
                }
            }
            else{
                if(iter2->second == '$'){
                    if(iter->first == estadoInicial) archivo<<iter->first<<" [color = \"blue\"];"<<endl;
                    else archivo<<iter->first<<" [color = \"red\"];"<<endl;
                }
                else{
                    if(iter->first == estadoInicial) archivo<<iter->first<<" [color = \"green\"];"<<endl;
                    else archivo<<iter->first<<";"<<endl;
                }
            }

        }
    }
    archivo<<"}";
    archivo.close();
    string comando = "dot -Tpdf eje.dot -o eje.pdf";
    system(comando.c_str());

}

void GramaticaRegular::imprimirTabla(){
    cout<<"  ";
    for(auto iter = tablaDeTransiciones.begin()->second.begin(); iter != tablaDeTransiciones.begin()->second.end(); ++iter){
        cout<<iter->first<<" ";
    }
    cout<<endl;
    for(auto iter = tablaDeTransiciones.begin(); iter != tablaDeTransiciones.end(); ++iter){
        cout<<iter->first<<" ";
        for(auto iter2 = tablaDeTransiciones.begin()->second.begin(); iter2 != tablaDeTransiciones.begin()->second.end(); ++iter2){
            cout<<iter->second[iter2->first]<<" ";
        }
        cout<<endl;
    }
}

GramaticaRegular::GramaticaRegular(string file){
    ifstream archivo(file.c_str());
    if(archivo.fail()){
	cout<<"El archivo "<<file<< "no se puede abrir o no existe"<<endl; 
	return;
    }
    char caracter;
    int estado = 0;
    char noTerminalTemp = ' ';
    bool ini = false;
    while(archivo.get(caracter)){
        switch(estado){
            case 0:
                if(caracter == 'N'){
                    archivo.get(caracter);
                    if(caracter == ':') estado = 1;
                    else{
                        cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                        return;
                    }
                }
                else{
                    cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                    return;
                }
                break;
            case 1:
                if(caracter == ';'){
                    estado = 2;
                    archivo.get(caracter);
                }
                else if(caracter != ','){
                    tablaDeTransiciones[caracter];
                    if(!ini){
                        estadoInicial = caracter;
                        ini = true;
                    }
                }
                break;
            case 2:
                if(caracter == 'T'){
                    archivo.get(caracter);
                    if(caracter == ':') estado = 3;
                    else{
                        cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                        return;
                    }
                }
                else{
                    cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                    return;
                }
                break;
            case 3:
                if(caracter == ';'){
                    estado = 4;
                    archivo.get(caracter);
                    for(auto iter = tablaDeTransiciones.begin(); iter != tablaDeTransiciones.end(); ++iter){
                        iter->second['%'] = '#';
                    }
                }
                else if(caracter != ','){
                    if(tablaDeTransiciones.find(caracter) != tablaDeTransiciones.end()){
                        cout<<"ERROR El caracter "<<caracter<<" ya existe en la lista de terminales. ABORTANDO"<<endl;
                        return;
                    }
                    for(auto iter = tablaDeTransiciones.begin(); iter != tablaDeTransiciones.end(); ++iter){
                        iter->second[caracter] = '#';
                    }
                }
                break;
            case 4:
                if(caracter == 'R'){
                    archivo.get(caracter);
                    if(caracter == ':'){
                        archivo.get(caracter);
                        estado = 5;
                    }
                    else{
                        cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                        return;
                    }
                }
                else{
                    cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                    return;
                }
                break;
            case 5:
                if(tablaDeTransiciones.find(caracter) != tablaDeTransiciones.end()){
                    noTerminalTemp = caracter;
                    archivo.get(caracter);
                    if(caracter == '-'){
                        archivo.get(caracter);
                        if(caracter == '>'){
                            estado = 6;
                            break;
                        }
                    }
                    cout<<"ERROR de sintaxis en "<<caracter<<" ABORTANDO"<<endl;
                    return;
                }
                else{
                    cout<<"El caracter "<<caracter<<"no esta en la lista de no terminales ABORTANDO";
                    return;
                }
            case 6:
                if(caracter == '%'){
                    tablaDeTransiciones[noTerminalTemp]['%'] = '$';
                    archivo.get(caracter);
                    estado = 5;
                }
                else if(tablaDeTransiciones.begin()->second.find(caracter) != tablaDeTransiciones.begin()->second.end()){
                    char terminalTemp = caracter;
                    archivo.get(caracter);
                    if(tablaDeTransiciones.find(caracter) != tablaDeTransiciones.end()){
                        tablaDeTransiciones[noTerminalTemp][terminalTemp] = caracter;
                        archivo.get(caracter);
                        estado = 5;
                    }
                    else{
                        cout<<"El caracter "<<caracter<<"no esta en la lista de no terminales ABORTANDO";
                        return;
                    }
                }
                else{
                    cout<<"ERROR en el caracter "<<caracter<<" ABORTANDO"<<endl;
                    return;
                }
                break;
        }
    }
    cout<<"Archivo leido Correctamente"<<endl;
}

GramaticaRegular::GramaticaRegular(){
    estadoInicial = '#';
}

#endif // GRAMATICAREGULAR_H
