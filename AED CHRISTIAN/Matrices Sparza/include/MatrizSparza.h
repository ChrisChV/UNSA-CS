#ifndef MATRIZSPARZA_H
#define MATRIZSPARZA_H
#include "RedBlack.h"
#include "algorithm"

template <typename T>
class MatrizSparza
{
    public:
        MatrizSparza();
        MatrizSparza(int,int,T);
        int fil_siz;
        int col_siz;
        T valorNulo;
        void print();
        void intercambiarFilas(int,int);
        void intercambiarColumnas(int,int);
        void agregarFila(int);
        void agregarColumna(int);
        void delFila(int);
        void delCol(int);
        RedBlack<int,T> & operator[](int);
        virtual ~MatrizSparza();
    protected:
    private:
        RedBlack<int,RedBlack<int, T> *> matriz;
};

template <typename T>
void MatrizSparza<T>::agregarColumna(int col){
    //if(col >= col_siz or col < -1)return;
    col_siz++;
    //for(int i = col_siz - 1; i > col + 1; i--){
      //  intercambiarColumnas(i, i - 1);
    //}
}

template <typename T>
void MatrizSparza<T>::agregarFila(int fil){
    if(fil >= fil_siz or fil < -1)return;
    fil_siz++;
    matriz[fil_siz - 1] = new RedBlack<int,T>();
    for(int i = fil_siz - 1; i > fil + 1; i--){
        intercambiarFilas(i, i - 1);
    }
}

template <typename T>
void MatrizSparza<T>::intercambiarColumnas(int col1, int col2){
    for(int i = 0; i < fil_siz; i++){
        if(matriz.find(i)){
            bool first = matriz[i]->find(col1);
            bool second = matriz[i]->find(col2);
            if(first and second){
                auto temp = (*matriz[i])[col1];
                (*matriz[i])[col1] = (*matriz[i])[col2];
                (*matriz[i])[col2] = temp;
            }
            else if(first){
                (*matriz[i])[col2] = (*matriz[i])[col1];
                matriz[i]->del(col1);
            }
            else if(second){
                (*matriz[i])[col1] = (*matriz[i])[col2];
                matriz[i]->del(col2);
            }
        }
    }
}

template <typename T>
void MatrizSparza<T>::delCol(int col){
    for(int i = 0; i < fil_siz; i++){
        if(matriz.find(i)){
            if(matriz[i]->find(col)){
                matriz[i]->del(col);
            }
        }
    }
}

template <typename T>
void MatrizSparza<T>::delFila(int fil){
    if(matriz.find(fil)) matriz.del(fil);
}

template <typename T>
void MatrizSparza<T>::intercambiarFilas(int fil1, int fil2){
    bool first = matriz.find(fil1);
    bool second = matriz.find(fil2);
    if(first and second){
        RedBlack<int,T> * temp = matriz[fil1];
        matriz[fil1] = matriz[fil2];
        matriz[fil2] = temp;
    }
    else if(first){
        matriz[fil2] = matriz[fil1];
        matriz.del(fil1);
    }
    else if(second){
        matriz[fil1] = matriz[fil2];
        matriz.del(fil2);
    }
}

template <typename T>
void MatrizSparza<T>::print(){
    for(int i = 0; i < fil_siz; i++){
        RedBlack<int, T> *temp;
        if(matriz.find(i,temp)){
            for(int j = 0; j < col_siz; j++){
                T caracter = valorNulo;
                temp->find(j,caracter);
                cout<<caracter;
            }
        }
        cout<<endl;
    }
}

template <typename T>
RedBlack<int,T> & MatrizSparza<T>::operator[](int fil){
    if(!matriz.find(fil)){
        matriz[fil] = new RedBlack<int,T>();
    }
    return *(matriz[fil]);
}

template <typename T>
MatrizSparza<T>::MatrizSparza(){
    fil_siz = 0;
    col_siz = 0;
}

template <typename T>
MatrizSparza<T>::MatrizSparza(int fil, int col, T nul){
    fil_siz = fil;
    col_siz = col;
    valorNulo = nul;
}

template <typename T>
MatrizSparza<T>::~MatrizSparza(){

}

#endif // MATRIZSPARZA_H
