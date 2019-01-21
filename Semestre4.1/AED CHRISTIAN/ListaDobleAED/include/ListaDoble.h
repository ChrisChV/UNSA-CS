#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#include "iostream"

using namespace std;

template <typename T>
class ListaDoble
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T);
                T dato;
                Nodo * siguiente;
                Nodo * atras;
                void destruirse();
        };
        void add(T);
        void print();
        ListaDoble();
        bool find(T, Nodo *&);
        void eliminar(T);
        bool isEmpty();
        virtual ~ListaDoble();
    protected:
    private:
        Nodo * cabeza;
        void _last(Nodo *&);
        void _print(Nodo *&);
};

template <typename T>
void ListaDoble<T>::Nodo::destruirse(){
    if(siguiente)
        siguiente->destruirse();
    delete this;
}

template <typename T>
bool ListaDoble<T>::isEmpty(){
    if(cabeza == nullptr)
        return true;
    return false;
}

template <typename T>
void ListaDoble<T>::eliminar(T dato){
    Nodo * nodo;
    if(!find(dato,nodo))return;
    Nodo *temp;
    _last(temp);
    if(nodo == cabeza){
        cabeza = nodo->siguiente;
    }
    else if(nodo == temp){
        nodo->atras->siguiente = nullptr;
    }
    else{
        nodo->atras->siguiente = nodo->siguiente;
        nodo->siguiente->atras = nodo->atras;
    }
    delete (nodo);
}

template <typename T>
bool ListaDoble<T>::find(T dato, Nodo *&nodo){
    nodo = cabeza;
    while(nodo != nullptr){
        if(nodo->dato == dato)return true;
        nodo = nodo->siguiente;
    }
    return false;
}

template <typename T>
void ListaDoble<T>::print(){
    _print(cabeza);
}

template <typename T>
void ListaDoble<T>::_print(Nodo *& nodo){
    if(nodo == nullptr)return;
    cout<<nodo->dato<<"<->";
    _print(nodo->siguiente);
}

template <typename T>
void ListaDoble<T>::_last(Nodo *& nodo){
    if(cabeza == nullptr){
        nodo = nullptr;
        return;
    }
    Nodo *temp = cabeza->siguiente;
    nodo = cabeza;
    while(temp != nullptr){
        nodo = temp;
        temp = temp->siguiente;
    }
}

template <typename T>
void  ListaDoble<T>::add(T dato){
    Nodo * nuevo = new Nodo(dato);
    if(cabeza == nullptr){
        cabeza = nuevo;
    }
    else{
        Nodo * nodo;
        _last(nodo);
        nuevo->atras = nodo;
        nodo->siguiente = nuevo;
    }
}

template <typename T>
ListaDoble<T>::ListaDoble(){
    cabeza = nullptr;
}
template<typename T>
ListaDoble<T>::Nodo::Nodo(){
    siguiente = nullptr;
    atras = nullptr;
}
template <typename T>
ListaDoble<T>::Nodo::Nodo(T dato){
    siguiente = nullptr;
    atras = nullptr;
    this->dato = dato;
}

template <typename T>
ListaDoble<T>::~ListaDoble(){
    cabeza->destruirse();
}

#endif // LISTADOBLE_H
