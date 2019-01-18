#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H
#include "iostream"
#include "list"
#include "FibonacciHeap.h"

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
        class NodoDTO{
            public:
                NodoDTO(){nodo = nullptr;};
                NodoDTO(Nodo * n){nodo = n;};
                bool operator ==(NodoDTO second){
                    if(nodo == second.nodo)return true;
                    return false;
                }
                bool operator !=(NodoDTO second){
                    if(nodo != second.nodo)return true;
                    return false;
                }
                bool operator >(NodoDTO second){
                    if(nodo->valor > second.nodo->valor)return true;
                    return false;
                }
                bool operator <(NodoDTO second){
                    if(nodo->valor < second.nodo->valor)return true;
                    return false;
                }
                Nodo * nodo;
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
        void uni(list<ArbolBinario>&);
        void print();
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
        void _uni(FibonacciHeap<NodoDTO>&);
};

void ArbolBinario::print(){
    ofstream archivo("eje.dot");
    if(archivo.fail()){
        cout<<"EL archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"digraph{";
    list<Nodo *> result;
    if(root) result.push_back(root);
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        archivo<<(*iter)->valor<<endl;
        if((*iter)->hijos[0]){
            archivo<<(*iter)->valor<<"->"<<(*iter)->hijos[0]->valor<<endl;
            result.push_back((*iter)->hijos[0]);
        }
        if((*iter)->hijos[1]){
            archivo<<(*iter)->valor<<"->"<<(*iter)->hijos[1]->valor<<endl;
            result.push_back((*iter)->hijos[1]);
        }
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf eje.dot -o eje.pdf");
}

void ArbolBinario::uni(list<ArbolBinario>& r){
    FibonacciHeap<NodoDTO> roots;
    for(ArbolBinario ab : r){
        roots.insert(NodoDTO(ab.root));
    }
    if(root){
        roots.insert(NodoDTO(root));
        root = nullptr;
    }
    return _uni(roots);
}

void ArbolBinario::_uni(FibonacciHeap<NodoDTO>& roots){
    if(roots.empty())return;
    if(root == nullptr){
        root = roots.popMin().nodo;
        return _uni(roots);
    }
    Nodo * menor = roots.popMin().nodo;
    Nodo * nuevo = new Nodo(root->valor + menor->valor);
    nuevo->hijos[0] = root;
    nuevo->hijos[1] = menor;
    root = nuevo;
    return _uni(roots);
}

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
    if(!root){
        root = new Nodo(valor);
        return;
    }
    list<Nodo *> result;
    result.push_back(root);
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        if((*iter)->hijos[0]){
            result.push_back((*iter)->hijos[0]);
        }
        else {
            (*iter)->hijos[0] = new Nodo(valor);
            return;
        }
        if((*iter)->hijos[1]){
            result.push_back((*iter)->hijos[1]);
        }
        else{
            (*iter)->hijos[1] = new Nodo(valor);
            return;
        }
    }
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
