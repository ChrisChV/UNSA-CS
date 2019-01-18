#ifndef COLATAREAS_H
#define COLATAREAS_H
#include "iostream"

using namespace std;

class ColaTareas
{
    public:
        class Tarea{
            public:
                Tarea();
                Tarea(int,string);
                Tarea * siguiente;
                Tarea * atras;
                int areaSolicitante;
                string descripcion;
        };
        ColaTareas();
        void add(int, string);
        void printTareas();
        void pop();
        Tarea * back();
        virtual ~ColaTareas();
    protected:
    private:
        Tarea * inicio;
        Tarea * cabeza;
};

void ColaTareas::printTareas(){
    if(!inicio){
        cout<<"Este emplado no tiene asignada ninguna tarea"<<endl;
        return;
    }
    Tarea * temp = cabeza;
    while(temp){
        cout<<"Area Solicitante: "<<temp->areaSolicitante<<endl;
        cout<<"Descripcion: "<<temp->descripcion<<endl;
        temp = temp->atras;
    }
    cout<<endl;
}

void ColaTareas::pop(){
    if(!inicio)return;
    auto temp = cabeza;
    if(inicio == cabeza){
        inicio = nullptr;
        cabeza = nullptr;
    }
    else{
        cabeza = cabeza->atras;
        cabeza->siguiente = nullptr;
    }
    delete(temp);
}

void ColaTareas::add(int area, string des){
    Tarea * nuevo = new Tarea(area, des);
    if(!inicio){
        inicio = nuevo;
        cabeza = nuevo;
    }
    else{
        nuevo->siguiente = inicio;
        inicio->atras = nuevo;
        inicio = nuevo;
    }
}

ColaTareas::Tarea::Tarea(){
    areaSolicitante = -1;
    descripcion = nullptr;
    siguiente = nullptr;
    atras = nullptr;
}

ColaTareas::Tarea::Tarea(int area, string descripcion){
    areaSolicitante = area;
    this->descripcion = descripcion;
    siguiente = nullptr;
    atras = nullptr;
}

ColaTareas::ColaTareas(){
    cabeza = nullptr;
    inicio = nullptr;
}

ColaTareas::~ColaTareas(){
}

#endif // COLATAREAS_H
