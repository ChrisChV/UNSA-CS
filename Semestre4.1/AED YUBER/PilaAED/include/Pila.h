#ifndef PILA_H
#define PILA_H

template<typename T>
class Pila
{
    public:
        Pila();
        class Nodo{
            public:
                Nodo();
                Nodo(T dato);
                T dato;
                Nodo *siguiente;
            private:
        };
        void agregar(T dato);
        bool esVacia();
        T desapilar();
    protected:
    private:
        Nodo * cabeza;
};

template<typename T>
Pila<T>::Nodo::Nodo(){
    siguiente = nullptr;
}
template<typename T>
Pila<T>::Nodo::Nodo(T dato){
    this->dato = dato;
}
template<typename T>
Pila<T>::Pila(){
    cabeza = nullptr;
}
template<typename T>
void Pila<T>::agregar(T dato){
    Pila<T>::Nodo * nuevo = new Nodo(dato);
    if(cabeza == nullptr){
        cabeza = nuevo;
    }
    else{
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
}
template<typename T>
bool Pila<T>::esVacia(){
    if(cabeza == nullptr)
        return true;
    return false;
}
template<typename T>
T Pila<T>::desapilar(){
    if(esVacia()){
        return 0;
    }
    else{
        T result = cabeza->dato;
        cabeza = cabeza->siguiente;
        return result;
    }
}



#endif // PILA_H
