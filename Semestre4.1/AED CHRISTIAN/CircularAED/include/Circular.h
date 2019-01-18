#ifndef CIRCULAR_H
#define CIRCULAR_H
#include "iostream"

using namespace std;

template <typename T>
class Circular
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T);
                T dato;
                Nodo * siguiente;
        };
        bool isEmpty();
        void insertar(T);
        void print();
        bool find(T);
        void eliminar(T);
        Circular();
        virtual ~Circular();
    protected:
    private:
        Nodo * cabeza;
        Nodo * cola;
        void _print(Nodo *&);
        void _colaLast(Nodo *&);
        bool _find(T, Nodo **&);
};

template <typename T>
void Circular<T>::eliminar(T dato){
    Nodo ** nodo;
    if(_find(dato,nodo))return;
    if(*nodo == cola){
        Nodo * temp;
        _colaLast(temp);
        cola = temp;
        cola->siguiente = cabeza;
    }
    else{
        *nodo = (*nodo)->siguiente;
    }
    delete (*nodo);
}

template <typename T>
void Circular<T>::_colaLast(Nodo *& nodo){
    if(isEmpty())return;
    nodo = cabeza;
    Nodo *temp = cabeza->siguiente;
    while(temp != cola){
        temp = temp->siguiente;
        nodo = nodo->siguiente;
    }
}

template<typename T>
bool Circular<T>::find(T dato){
    Nodo ** nodo = &cabeza;
    return _find(dato, nodo);
}

template<typename T>
bool Circular<T>::_find(T dato, Nodo **& nodo){
    while(*nodo != cabeza){
        if((*nodo)->dato == dato)return true;
        nodo = &((*nodo)->siguiente);
    }
    return false;
}

template<typename T>
void Circular<T>::insertar(T dato){
    Nodo * nuevo = new Nodo(dato);
    if(isEmpty()){
        cabeza = nuevo;
        cola = nuevo;
        nuevo->siguiente = nuevo;
    }
    else{
        cola->siguiente = nuevo;
        nuevo->siguiente = cabeza;
        cola = nuevo;
    }
}

template<typename T>
void Circular<T>::_print(Nodo *& nodo){
    if(nodo == cola){
        cout<<nodo->dato;
        return;
    }
    cout<<nodo->dato<<"->";
    _print(nodo->siguiente);
}

template<typename T>
void Circular<T>::print(){
    _print(cabeza);
}
template<typename T>
Circular<T>::~Circular(){
    Nodo * nodo = cabeza;
    if(isEmpty())return;
    while(nodo != cola){
        Nodo * temp = nodo;
        nodo = nodo->siguiente;
        delete(temp);
    }
}

template<typename T>
bool Circular<T>::isEmpty(){
    if(cabeza == nullptr)
        return true;
    return false;
}



template<typename T>
Circular<T>::Nodo::Nodo(){
    siguiente = nullptr;
}
template<typename T>
Circular<T>::Nodo::Nodo(T dato){
    siguiente = nullptr;
    this->dato = dato;
}
template<typename T>
Circular<T>::Circular(){
    cabeza = nullptr;
    cola = nullptr;
}

#endif // CIRCULAR_H
