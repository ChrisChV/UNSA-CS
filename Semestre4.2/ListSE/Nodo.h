#ifndef NODO_H
#define NODO_H

template<class T>
class Nodo{
    public:
        T dato;
        Nodo<T> *sig;
        Nodo();
        Nodo(T);

};

template<class T>
Nodo<T> :: Nodo(){
    sig = 0;
}

template<class T>
Nodo<T> :: Nodo(T d){
    sig = 0;
    dato = d;
}

#endif // NODO_H
