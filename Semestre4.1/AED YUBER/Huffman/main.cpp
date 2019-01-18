#include <iostream>
#include "ArbolH.h"
#include "map"

using namespace std;

void comprimir(string file){
    ifstream archivo(file);
    if(archivo.fail()){
        cout<<"EL archivo no se puede abrir"<<endl;
        return;
    }
    map<char,int> alfabeto;
    float total = 0;
    char caracter;
    while(archivo.get(caracter)){
        if(alfabeto.find(caracter) == alfabeto.end()){
            alfabeto[caracter] = 1;
        }
        else{
            int temp = alfabeto[caracter];
            alfabeto[caracter] = temp + 1;
        }
        total++;
    }
    vector<ArbolH::NodoDTO> nodos;
    for(auto iter = alfabeto.begin(); iter != alfabeto.end(); ++iter){
        cout<<"ALFA->"<<iter->first<<endl;
        cout<<iter->first<<endl;
        cout<<iter->second / total<<endl;
        minHeapinsert(nodos,ArbolH::NodoDTO(new ArbolH::Nodo(iter->first,iter->second / total)));
    }
    ArbolH arbolito;
    arbolito.uni(nodos);
    arbolito.imprimirAlfabeto();
    archivo.close();
    ifstream archivo2(file);
    ofstream salida("salida.txt");
    string ar = arbolito.reducirArbol();
    salida<<ar<<endl;
    while(archivo2.get(caracter)){
        string cod = arbolito.codificar(caracter);
        cout<<caracter<<endl;
        cout<<cod<<endl;
        salida<<cod;
    }
    archivo2.close();
    salida.close();
}

void descomprimir(string file){
    ifstream archivo("salida.txt");
    if(archivo.fail()){
        cout<<"El archvibo no puede abrirse"<<endl;
        return;
    }
    char caracter[200];
    archivo.getline(caracter,200);
    ArbolH arbolito;
    string cod(caracter);
    arbolito.generarArbol(cod);
    arbolito.imprimirAlfabeto();
    ofstream archivo2(file);
    char c = 'a';
    char ca;
    archivo.get(c);
    while(true){
        c = arbolito.decodificar(archivo,ca);
        if(c == CARACTER_NULL) break;
        cout<<c<<endl;
        archivo2<<c;
    }
    archivo2.close();
}

int main()
{
    comprimir("texto11.txt");
    descomprimir("texto2.txt");
}
