#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H
#include "iostream"
#include "list"

using namespace std;

class ArbolBinario
{
    public:
        class Nodo{
            public:
            Nodo();
            Nodo(int);
            int valor;
            Nodo * hijos[2];
        };
        ArbolBinario();
        void amplitud();
        void amplitudInverso();
        void insert(int);
        void postorden();
        void preorden();
        void inorden();
        void inordenInverso();
        void postordenInverso();
        void preordenInverso();
        virtual ~ArbolBinario();
    protected:
    private:
        void _postorden(Nodo *&);
        void _inorden(Nodo *&);
        void _preorden(Nodo *&);
        void _inordenInverso(Nodo *&);
        void _postordenInverso(Nodo *&);
        void _preordenInverso(Nodo *&);
        Nodo * root;

};

void ArbolBinario::amplitudInverso(){
    if(!root)return;
    list<Nodo *> result;
    result.push_back(root);
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        if((*iter)->hijos[0]){
            result.push_back((*iter)->hijos[0]);
        }
        if((*iter)->hijos[1]){
            result.push_back((*iter)->hijos[1]);
        }
    }
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        cout<<(*iter)->valor<<endl;
    }
};

void ArbolBinario::preordenInverso(){
    _preordenInverso(root);
}

void ArbolBinario::postordenInverso(){
    _postordenInverso(root);
}
void ArbolBinario::inordenInverso(){
    _inordenInverso(root);
}

void ArbolBinario::_preordenInverso(Nodo *&nodo){
    if(!nodo)return;
    cout<<nodo->valor<<endl;
    _preordenInverso(nodo->hijos[1]);
    _preordenInverso(nodo->hijos[0]);
}

void ArbolBinario::_postordenInverso(Nodo *&nodo){
    if(!nodo)return;
    _postordenInverso(nodo->hijos[1]);
    _postordenInverso(nodo->hijos[0]);
    cout<<nodo->valor<<endl;
}

void ArbolBinario::_inordenInverso(Nodo *&nodo){
    if(!nodo)return;
    _inordenInverso(nodo->hijos[1]);
    cout<<nodo->valor<<endl;
    _inordenInverso(nodo->hijos[0]);
}
void ArbolBinario::amplitud(){
    if(!root)return;
    list<Nodo *> result;
    result.push_back(root);
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        if((*iter)->hijos[0]){
            result.push_back((*iter)->hijos[0]);
        }
        if((*iter)->hijos[1]){
            result.push_back((*iter)->hijos[1]);
        }
    }
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        cout<<(*iter)->valor<<endl;
    }
}

void ArbolBinario::preorden(){
    _preorden(root);
}

void ArbolBinario::inorden(){
    _inorden(root);
}

void ArbolBinario::postorden(){
    _postorden(root);
}

void ArbolBinario::_postorden(Nodo *& nodo){
    if(!nodo)return;
    _postorden(nodo->hijos[0]);
    _postorden(nodo->hijos[1]);
    cout<<nodo->valor<<endl;
}

void ArbolBinario::_inorden(Nodo *& nodo){
    if(!nodo)return;
    _inorden(nodo->hijos[0]);
    cout<<nodo->valor<<endl;
    _inorden(nodo->hijos[1]);
}

void ArbolBinario::_preorden(Nodo *&nodo){
    if(!nodo)return;
    cout<<nodo->valor<<endl;
    _preorden(nodo->hijos[0]);
    _preorden(nodo->hijos[1]);
}

void ArbolBinario::insert(int valor){
    Nodo ** iter = &(root);
    while(*iter){
        iter = &((*iter)->hijos[(*iter)->valor <= valor]);
    }
    *iter = new Nodo(valor);
}

ArbolBinario::Nodo::Nodo(){
    valor = 0;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
}

ArbolBinario::Nodo::Nodo(int valor){
    this->valor = valor;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
}

ArbolBinario::ArbolBinario(){
    root = nullptr;
}

ArbolBinario::~ArbolBinario(){}


#endif // ARBOLBINARIO_H
