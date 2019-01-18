#ifndef TREE_H
#define TREE_H
#include "iostream"
#include "list"
#include "algorithm"
#include "fstream"

using namespace std;

template <typename T>
class Tree
{
    public:
        Tree();
        class Nodo{
            public:
                Nodo();
                Nodo(T dato);
                T dato;
                Nodo * hijos[2];
                void destruir();
        };
        void print();
        void printN();
        void printPreorden();
        void printInorden();
        void printPostorden();
        int alturaR();
        int altura();
        int numeroHojas();
        int numeroNodos();
        bool find(T, Nodo **&);
        bool findR(T);
        bool insert(T dato);
        void del(T valor);
        virtual ~Tree();
        void printDot();
    protected:
    private:
        void _printPreorden(Nodo *);
        void _printInorden(Nodo *);
        void _printPostorden(Nodo *);
        bool _findR(T, Nodo *);
        int _numeroHojas(Nodo* ,int);
        int _numeroNodos(Nodo *);
        int _altura(Nodo *,int);
        void _printN(list<Nodo*> lista);
        void _menorIzq(Nodo **&);
        Nodo * root;
};

template <typename T>
void Tree<T>::printDot(){
    ofstream archivo("eje.dot");
    if(archivo.fail()){
        cout<<"EL archivo no se pudo abrir"<<endl;
        return;
    }
    archivo<<"digraph{";
    list<Nodo *> result;
    if(root) result.push_back(root);
    for(auto iter = result.begin(); iter != result.end(); ++iter){
        archivo<<(*iter)->dato<<endl;
        if((*iter)->hijos[0]){
            archivo<<(*iter)->dato<<"->"<<(*iter)->hijos[0]->dato<<endl;
            result.push_back((*iter)->hijos[0]);
        }
        if((*iter)->hijos[1]){
            archivo<<(*iter)->dato<<"->"<<(*iter)->hijos[1]->dato<<endl;
            result.push_back((*iter)->hijos[1]);
        }
    }
    archivo<<"}";
    archivo.close();
    system("dot -Tpdf eje.dot -o eje.pdf");
}

template <typename T>
void Tree<T>::_menorIzq(Nodo ** &nodo){
    nodo = &((*nodo)->hijos[0]);
    while(true){
        if(!(*nodo)->hijos[1])return;
        nodo = &((*nodo)->hijos[1]);
    }
}

template <typename T>
void Tree<T>::del(T valor){
    Nodo ** nodo;
    if(!this->find(valor,nodo))return;
    if((*nodo)->hijos[0] and (*nodo)->hijos[1]){
        Nodo ** temp = nodo;
        _menorIzq(temp);
        swap((*temp)->dato, (*nodo)->dato);
        nodo = temp;
    }
    if((*nodo)->hijos[0]){
        *nodo = (*nodo)->hijos[0];
    }
    else if((*nodo)->hijos[1]){
        *nodo = (*nodo)->hijos[1];
    }
    else{
        *nodo = nullptr;
    }
}

template <typename T>
void Tree<T>::printN(){
    list<Nodo*> lista;
    lista.push_back(root);
    _printN(lista);
}


template <typename T>
void Tree<T>::_printN(list<Nodo*> lista){
    if(lista.empty())return;
    list<Nodo*> temp;
    for(auto iter = lista.begin(); iter != lista.end(); ++iter){
        if((*iter)->hijos[0]) temp.push_back((*iter)->hijos[0]);
        if((*iter)->hijos[1]) temp.push_back((*iter)->hijos[1]);
    }
    _printN(temp);
    for(auto iter = lista.begin(); iter != lista.end(); ++iter){
        cout<<(*iter)->dato<<endl;
    }
}

template <typename T>
int Tree<T>::numeroNodos(){
    return _numeroNodos(root);
}

template <typename T>
int Tree<T>::_numeroNodos(Nodo *nodo){
    if(!nodo)return 0;
    int a = _numeroNodos(nodo->hijos[0]);
    int b = _numeroNodos(nodo->hijos[1]);
    return a + b + 1;
}

template <typename T>
int Tree<T>::altura(){
    if(!root)return -1;
    int resultado = -1;
    list<Nodo*> lista;
    lista.push_back(root);
    while(!lista.empty()){
        resultado++;
        list<Nodo*> temp;
        for(auto iter = lista.begin(); iter != lista.end(); ++iter){
            if((*iter)->hijos[0]) temp.push_back((*iter)->hijos[0]);
            if((*iter)->hijos[1]) temp.push_back((*iter)->hijos[1]);
        }
        lista = temp;
    }
    return resultado;
}

template <typename T>
int Tree<T>::numeroHojas(){
    return _numeroHojas(root, 0);
}

template <typename T>
int Tree<T>::_numeroHojas(Nodo* nodo, int resultado){
    if(!nodo)return resultado;
    if(nodo->hijos[0] == nullptr and nodo->hijos[1] == nullptr)return resultado + 1;
    int a = _numeroHojas(nodo->hijos[0], resultado);
    int b = _numeroHojas(nodo->hijos[1], resultado);
    return a + b;
}

template <typename T>
int Tree<T>::alturaR(){
    return _altura(root,0);
}

template <typename T>
int Tree<T>::_altura(Nodo *nodo, int resultado){
    if(!nodo)return resultado - 1;
    int derecho = _altura(nodo->hijos[1], resultado + 1);
    int izquierdo = _altura(nodo->hijos[0], resultado + 1);
    if(derecho > izquierdo)return derecho;
    return izquierdo;
}

template <typename T>
void Tree<T>::print(){
    if(!root)return;
    list<Nodo*> lista;
    lista.push_back(root);
    while(!lista.empty()){
        list<Nodo *>temp;
        for(auto iter = lista.begin(); iter != lista.end(); ++iter){
            cout<<(*iter)->dato<<endl;
            if((*iter)->hijos[0])temp.push_back((*iter)->hijos[0]);
            if((*iter)->hijos[1])temp.push_back((*iter)->hijos[1]);
        }
        lista = temp;
    }
}

template <typename T>
void Tree<T>::printPostorden(){
    _printPostorden(root);
}

template <typename T>
void Tree<T>::_printPostorden(Nodo * nodo){
    if(!nodo) return;
    _printPostorden(nodo->hijos[0]);
    _printPostorden(nodo->hijos[1]);
    cout<<nodo->dato<<endl;
}

template <typename T>
void Tree<T>::printInorden(){
    _printInorden(root);
}

template <typename T>
void Tree<T>::_printInorden(Nodo * nodo){
    if(!nodo)return;
    _printInorden(nodo->hijos[0]);
    cout<<nodo->dato<<endl;
    _printInorden(nodo->hijos[1]);
}


template <typename T>
bool Tree<T>::findR(T dato){
    return _findR(dato, root);
}

template <typename T>
bool Tree<T>::_findR(T dato, Nodo * nodo){
    if(!nodo)return false;
    if(nodo->dato == dato)return true;
    _findR(dato, nodo->hijos[nodo->dato < dato]);
}

template <typename T>
void Tree<T>::printPreorden(){
    _printPreorden(root);
}

template <typename T>
void Tree<T>::Nodo::destruir(){
    if(hijos[0])hijos[0]->destruir();
    if(hijos[1])hijos[1]->destruir();
    delete this;
}

template <typename T>
void Tree<T>::_printPreorden(Nodo *nodo){
    if(nodo == nullptr)return;
    cout<<nodo->dato<<endl;
    _printPreorden(nodo->hijos[0]);
    _printPreorden(nodo->hijos[1]);
}

template <typename T>
bool Tree<T>::insert(T dato){
    Nodo **nodo;
    if(find(dato,nodo))return false;
    *nodo = new Nodo(dato);
    return true;
}

template <typename T>
bool Tree<T>::find(T dato, Nodo **&nodo){
    nodo = &root;
    while(*nodo != nullptr){
        if((*nodo)->dato == dato)return true;
        nodo = &((*nodo)->hijos[(*nodo)->dato < dato]);
    }
    return false;
}

template <typename T>
Tree<T>::Tree(){
    root = nullptr;
}

template <typename T>
Tree<T>::Nodo::Nodo(){
    hijos[0] = nullptr;
    hijos[2] = nullptr;
}

template <typename T>
Tree<T>::Nodo::Nodo(T dato){
    this->dato = dato;
    hijos[0] = nullptr;
    hijos[1] = nullptr;
}

template <typename T>
Tree<T>::~Tree(){
    if(root) root->destruir();
}

#endif // TREE_H
