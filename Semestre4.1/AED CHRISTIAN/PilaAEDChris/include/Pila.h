#ifndef PILA_H
#define PILA_H
#include "iostream"

using namespace std;

template <typename T>
class Pila
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T);
                T dato;
                Nodo *siguiente;
                void destruir();
        };
        Pila();
        void push(T);
        void print();
        void pop();
        bool isEmpty();
        T top();
        virtual ~Pila();
    protected:
    private:
        Nodo * cabeza;
        void _print(Nodo *& nodo);


};

template <typename T>
Pila<T>::~Pila(){
    cabeza->destruir();
}

template <typename T>
void Pila<T>::Nodo::destruir(){
    if(this == nullptr)return;
    if(siguiente){
        siguiente->destruir();
    }
    delete this;
}

template <typename T>
T Pila<T>::top(){
    return cabeza->dato;
}

template <typename T>
bool Pila<T>::isEmpty(){
    if(cabeza == nullptr)return true;
    return false;
}

template <typename T>
void Pila<T>::pop(){
    if(isEmpty())return;
    auto temporal = cabeza;
    cabeza = temporal->siguiente;
    delete temporal;
}

template <typename T>
Pila<T>::Nodo::Nodo(){
    siguiente = nullptr;
}
template <typename T>
Pila<T>::Nodo::Nodo(T dato){
    this->dato = dato;
    siguiente = nullptr;
}
template <typename T>
Pila<T>::Pila(){
    cabeza = nullptr;
}
template <typename T>
void Pila<T>::push(T dato){
    Pila<T>::Nodo * nuevo = new Nodo(dato);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}
template <typename T>
void Pila<T>::_print(Nodo *& nodo){
    if(nodo == nullptr) return;
    cout<<nodo->dato<<"->";
    _print(nodo->siguiente);
}
template <typename T>
void Pila<T>::print(){
    _print(cabeza);
}
#endif // PILA_H
