#ifndef COLA_H
#define COLA_H
#include "iostream"

using namespace std;

template <typename T>
class Cola
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T);
                T dato;
                Nodo * siguiente;
                void destruir();
        };
        Cola();
        void encolar(T);
        void desencolar();
        void print();
        bool isEmpty();
        T front();
        virtual ~Cola();
    protected:
    private:
        Nodo * cabeza;
        void _last(Nodo **&);
        void _print(Nodo *&);
};

template <typename T>
T Cola<T>::front(){
    if(isEmpty())return 0;
    Nodo **nodo = &cabeza;
    _last(nodo);
    return (*nodo)->dato;
}

template <typename T>
bool Cola<T>::isEmpty(){
    if(cabeza == nullptr)
        return true;
    return false;
}

template <typename T>
void Cola<T>::_last(Nodo **& nodo){
    Nodo * sigTemp = cabeza->siguiente;
    while(sigTemp != nullptr){
        nodo = &((*nodo)->siguiente);
        sigTemp =  sigTemp->siguiente;
    }
}

template <typename T>
void Cola<T>::desencolar(){
    if(isEmpty())return;
    Nodo ** nodo = &cabeza;
    _last(nodo);
    *nodo = nullptr;
    delete(*nodo);
}

template <typename T>
Cola<T>::~Cola(){
    cabeza->destruir();
}

template <typename T>
void Cola<T>::Nodo::destruir(){
    if(this == nullptr)return;
    if(siguiente)
        siguiente->destruir();
    delete this;
}

template <typename T>
void Cola<T>::print(){
    _print(cabeza);
}
template <typename T>
void Cola<T>::_print(Nodo *& nodo){
    if(nodo == nullptr) return;
    cout<<nodo->dato<<"->";
    _print(nodo->siguiente);
}


template <typename T>
void Cola<T>::encolar(T dato){
    Cola<T>::Nodo * nuevo = new Nodo(dato);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

template<typename T>
Cola<T>::Nodo::Nodo(){
    siguiente = nullptr;
}

template <typename T>
Cola<T>::Nodo::Nodo(T dato){
    siguiente = nullptr;
    this->dato = dato;
}

template <typename T>
Cola<T>::Cola(){
    cabeza = nullptr;
}

#endif // COLA_H
