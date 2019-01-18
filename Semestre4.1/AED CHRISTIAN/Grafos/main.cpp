#include <iostream>
#include "map"
#include "vector"
#include "algorithm"
#include "fstream"
#include "Grafo.h"

/*
#define INFINITO 10000;

        if(valores[i] == a)return i;
    }
    return -1;
using namespace std;

typedef map<int,map<int,float>> Grafo;
    }
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
            if(find(valores.begin(), valores.end(), iter2->first) == valores.end())return false;
        }
    }
    return true;
}

void printG(Grafo grafo){
    vector<int> valores;
    if(!esGrafo(grafo,valores))return;
    ofstream archivo("grafo.dot");
    if(archivo.fail()){
        cout<<"El archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"graph{"<<endl;
    for(auto iter = grafo.begin(); iter != grafo.end(); ++iter){
        archivo<<iter->first<<endl;
        for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
            archivo<<iter->first<<" -- "<<iter2->first<<" [label=\""<<iter2->second<<"\"];"<<endl;
        }
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf grafo.dot -o grafo.pdf");
}

void warshall(Grafo &grafo){
    for(int i = 0; i < grafo.size(); i++){
        for(int j = 0 ; j < grafo.size(); j++){
            for(int k = 0; k < grafo.size(); k++){
                if(grafo[i].find(j) != grafo[i].end() or (grafo[i].find(k) != grafo[i].end() and grafo[k].find(j) != grafo[k].end())){
                    grafo[i][j] = 1;
                }
            }
        }
    }
}

void floid(Grafo &grafo){
    for(int i = 0; i < grafo.size(); i++){
        for(int j = 0; j < grafo.size(); j++){
            for(int k = 0; k < grafo.size(); k++){
                int a = INFINITO;
                int b = INFINITO;
                int c = INFINITO;
                if(grafo[i].find(j) != grafo[i].end()) a = grafo[i][j];
                if(grafo[i].find(k) != grafo[i].end()) b = grafo[i][k];
                if(grafo[k].find(j) != grafo[k].end()) c = grafo[k][j];
                if(a > b + c){
                    grafo[i][j] = b + c;
                }
            }
        }
    }
}

void generarVectores(Grafo &grafo, vector<int> &valores, map<int,int> &distancias, int i, map<int,bool> &candidatos){
    for(int j = 0; j < valores.size(); j++){
        if(grafo[i].find(valores[j]) != grafo[i].end()){
            distancias[valores[j]] = grafo[i][valores[j]];
        }
        else{
            distancias[valores[j]] = INFINITO;
        }
        candidatos[valores[j]] = false;
    }
}

int getMin(map<int, bool> &candidatos, map<int,int> &distancias){
    int menor = INFINITO;
    menor++;
    int resultado = -1;
    for(auto iter = candidatos.begin(); iter != candidatos.end(); ++iter){
        if(!iter->second){
            if(menor > distancias[iter->first]){
                menor = distancias[iter->first];
                resultado  = iter->first;
            }
        }
    }
    return resultado;
}

bool hayCero(map <int,bool> &candidatos){
    for(auto iter = candidatos.begin(); iter != candidatos.end(); ++iter){
        if(iter->second == false)return true;
    }
    return false;
}

map<int,int> dijkstra(Grafo &grafo, int i){
    vector<int> valores;
    map<int,int> distancias;
    if(!esGrafo(grafo,valores))return distancias;
    map<int,bool> candidatos;
    generarVectores(grafo,valores,distancias,i,candidatos);
    candidatos[i] = true;
    while(hayCero(candidatos)){
        int p = getMin(candidatos,distancias);
        candidatos[p] = true;
        for(auto iter = distancias.begin(); iter != distancias.end(); ++iter){
            if(candidatos[iter->first] == false){
                int q = INFINITO;
                if(grafo[p].find(iter->first) != grafo[p].end()) q = grafo[p][iter->first];
                distancias[iter->first] = min(distancias[iter->first], distancias[p] + q);
            }
        }
    }
    return distancias;
}
*/

void imprimir(MyGrafo::Nodo * nodo){
    cout<<"VALOR->"<<nodo->nombre<<endl;
}

int main()
{
    MyGrafo g;

    g["0"];
    g["1"];
    g["2"];
    g["3"];
    g["4"];
    g["5"];
    g["6"];
    g("0","1",3);
    g("0","2",5);
    g("1","2",1);
    g("1","3",10);
    g("1","4",21);
    g("2","3",1);
    g("2","5",2);
    g("3","5",1);
    g("3","4",7);
    g("4","5",2);
    g("6","2",2);

    /*
    g["0"];
    g["1"];
    g["2"];
    g["3"];
    g["4"];
    g("0","1",2);
    g("0","4",100);
    g("1","4",7);
    g("1","2",5);
    g("2","0",18);
    g("2","3",1);
    g("2","4",9);
    g("3","4",1);
    auto dis = g.dijkstra("0");
    for(auto iter = dis.begin(); iter != dis.end(); ++iter){
        cout<<iter->first<<"->"<<iter->second<<endl;
    }
    */

    //g.print();

    //void (*fun)(MyGrafo::Nodo *);
    //fun = imprimir;
    //g.BSF(fun);
    //g.DSF(fun);
    MyGrafo arbolito;
    g.prim(arbolito);
    arbolito.print();

}
