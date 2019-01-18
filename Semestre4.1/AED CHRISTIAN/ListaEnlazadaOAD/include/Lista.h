#ifndef LISTA_H
#define LISTA_H
#include "iostream"

using namespace std;

template <typename T>
class Lista
{
    public:
        class Nodo{
            public:
                Nodo();
                Nodo(T dato);
                T dato;
                Nodo *siguiente;
            private:
        };
        Lista();
        void print();
        int size();
        void eliminar(T dato);
        int sizePar();
        void _printR(Nodo *A);
        void printR();
        void push_back(T);
        void push_front(T);
        void insert_sort(T);
        T max();
        void reverse();
        bool find(T d);
        Lista<T> unio(Lista<T> &);
        Lista<T> intersect(Lista<T> &);
    protected:
    private:
        Nodo * cabeza;
        Nodo * cola;
};

template <typename T>
void Lista<T>::eliminar(T dato){
    if(cabeza == nullptr){
        return;
    }
    Lista<T>::Nodo * temporal = cabeza;
    Lista<T>::Nodo * sigTemporal = cabeza;
    if(dato == temporal->dato){
        cabeza = cabeza->siguiente;
        delete(temporal);
        return;
    }
    sigTemporal = cabeza;
    while(sigTemporal != cola){
        if(dato == sigTemporal->dato){
            temporal->siguiente = sigTemporal->siguiente;
            delete(sigTemporal);
            return;
        }
        temporal = temporal->siguiente;
        sigTemporal = sigTemporal->siguiente;
    }
    if(dato == sigTemporal->dato){
        cola = temporal;
        temporal->siguiente = nullptr;
        delete(sigTemporal);
        return;
    }
}

template <typename T>
Lista<T> Lista<T>::intersect(Lista<T> &segundo){
    Lista<T> resultado;
    if(size() > segundo.size()){
        Lista<T>::Nodo * temporal = cabeza;
        while(temporal != nullptr){
            if(segundo.find(temporal->dato) and !resultado.find(temporal->dato)){
                resultado.insert_sort(temporal->dato);
            }
            temporal = temporal->siguiente;
        }
    }
    else{
        Lista<T>::Nodo * temporal = segundo.cabeza;
        while(temporal != nullptr){
            if(find(temporal->dato) and !resultado.find(temporal->dato)){
                resultado.insert_sort(temporal->dato);
            }
            temporal = temporal->siguiente;
        }
    }
    return resultado;
}

template <typename T>
bool Lista<T>::find(T d){
    Lista<T>::Nodo * temporal = cabeza;
    while(temporal != nullptr){
        if(temporal->dato == d){
            return true;
        }
        temporal = temporal->siguiente;
    }
    return false;
}

template <typename T>
Lista<T> Lista<T>::unio(Lista<T> &segunda){
    Lista<T> resultado;
    Lista<T>::Nodo * temporal = cabeza;
    while(temporal != nullptr){
        if(!resultado.find(temporal->dato)){
            resultado.insert_sort(temporal->dato);
        }
        temporal = temporal->siguiente;
    }
    temporal = segunda.cabeza;
    while(temporal != nullptr){
        if(!resultado.find(temporal->dato)){
            resultado.insert_sort(temporal->dato);
        }
        temporal = temporal->siguiente;
    }
    return resultado;
}

template<typename T>
void Lista<T>::reverse(){
    if(cabeza == nullptr){
        return;
    }
    Lista<T>::Nodo * iterador = cabeza;
    Lista<T>::Nodo * iteradorSig = cabeza->siguiente;
    cabeza->siguiente = nullptr;
    cola = cabeza;
    while(iteradorSig != nullptr){
        Lista<T>::Nodo * temporal = iteradorSig->siguiente;
        iteradorSig->siguiente = iterador;
        iterador = iteradorSig;
        iteradorSig = temporal;
    }
    cabeza = iterador;
}

template<typename T>
T Lista<T>::max(){
    Lista<T>::Nodo *temporal = cabeza;
    T mayor = 0;
    while(temporal != nullptr){
        if(mayor < temporal->dato){
            mayor = temporal->dato;
        }
        temporal = temporal->siguiente;
    }
    return mayor;
}

template<typename T>
void Lista<T>::insert_sort(T d){
    Lista<T>::Nodo * nuevo = new Nodo(d);
    if(cabeza == nullptr){
        cabeza = nuevo;
        cola = nuevo;
        return;
    }
    Lista<T>::Nodo * temporal = cabeza;
    Lista<T>::Nodo * temporalSig = temporal;
    if(temporal->dato > nuevo->dato){
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        return;
    }
    temporalSig = temporalSig->siguiente;
    while(temporalSig != nullptr){
        if(temporalSig->dato > nuevo->dato){
            nuevo->siguiente = temporalSig;
            temporal->siguiente = nuevo;
            return;
        }
        temporal = temporalSig;
        temporalSig = temporalSig->siguiente;
    }
    push_back(d);
}

template<typename T>
void Lista<T>::push_back(T d){
    Lista<T>::Nodo * nuevo = new Nodo(d);
    if(cabeza == nullptr){
        cabeza = nuevo;
        cola = nuevo;
    }
    else{
        cola->siguiente = nuevo;
        cola = nuevo;
    }
}

template<typename T>
void Lista<T>::push_front(T d){
    Lista<T>::Nodo * nuevo = new Nodo(d);
    if(cabeza == nullptr){
        cabeza = nuevo;
        cola = nuevo;
    }
    else{
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
}

template<typename T>
void Lista<T>::print(){
    Lista<T>::Nodo * temporal = cabeza;
    while(temporal != nullptr){
        cout<<temporal->dato<<"->";
        temporal = temporal->siguiente;
    }
}

template<typename T>
int Lista<T>::size(){
    Lista<T>::Nodo * temporal = cabeza;
    int contador = 0;
    while(temporal != nullptr){
        contador++;
        temporal = temporal->siguiente;
    }
    return contador;
}

template<typename T>
int Lista<T>::sizePar(){
    Lista<T>::Nodo * temporal = cabeza;
    int contador = 0;
    while(temporal != nullptr){
        if(temporal->dato % 2 == 0)
            contador++;
        temporal = temporal->siguiente;
    }
    return contador;
}

template<typename T>
void Lista<T>::printR(){
    _printR(cabeza);
}

template<typename T>
void Lista<T>::_printR(Nodo * A){
    if(A != cola){
        _printR(A->siguiente);
    }
    cout<<A->dato<<"->";
}



template <typename T>
Lista<T>::Nodo::Nodo(){
    dato = nullptr;
    siguiente = nullptr;
}

template <typename T>
Lista<T>::Nodo::Nodo(T dato){
    this->dato = dato;
    siguiente = nullptr;
}

template <typename T>
Lista<T>::Lista(){
    cabeza = nullptr;
    cola = nullptr;
}

#endif // LISTA_H
