#ifndef GRAFO_H
#define GRAFO_H
#include "vector"
#include "map"

using namespace std;

class GrafoDirigido
{
    public:
        GrafoDirigido();
        GrafoDirigido(vector<vector<int>>);
        int size();
        vector<int> operator [] (int);
        virtual ~GrafoDirigido();
    protected:
    private:
        vector<vector<int>> matriz;
        int siz;
};

int GrafoDirigido::size(){
    return this->siz;
}

vector<int> GrafoDirigido::operator[] (int index){
    return matriz[index];
}

GrafoDirigido::Grafo(vector<vector<int>> m){
    matriz = m;
    siz = matriz.size();
}

GrafoDirigido::Grafo(){

}

GrafoDirigido::~GrafoDirigido(){

}

#endif // GRAFO_H
