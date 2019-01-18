#ifndef LISTAEMPLEADOS_H
#define LISTAEMPLEADOS_H
#include "ColaTareas.h"
#include "iostream"

using namespace std;

class ListaEmpleados
{
    public:
        class Empleado{
            public:
                Empleado();
                Empleado(int,string);
                string nombre;
                int codEmpleado;
                int cantidadTareas;
                ColaTareas tareas;
                Empleado * siguiente;
        };
        ListaEmpleados();
        void addEmpleado(string);
        void addTarea(int, string);
        void addTarea(int,int,string);
        void printMax();
        void printMin();
        bool find(int, Empleado *&);
        Empleado * min();
        Empleado * max();
        virtual ~ListaEmpleados();
    protected:
    private:
        Empleado *inicio;
        Empleado *fin;
        int tam;
};

void ListaEmpleados::printMin(){
    if(!inicio){
        cout<<"No existe ningun empleado"<<endl;
        return;
    }
    Empleado * menor = this->min();
    cout<<"EL empleado con menos tareas es: "<<menor->nombre<<endl;
    cout<<"Tareas:"<<endl;
    menor->tareas.printTareas();
    cout<<endl;
}

void ListaEmpleados::printMax(){
    if(!inicio){
        cout<<"No existe ningun empleado"<<endl;
        return;
    }
    Empleado * mayor = this->max();
    cout<<"EL empleado con mas tareas es: "<<mayor->nombre<<endl;
    cout<<"Tareas:"<<endl;
    mayor->tareas.printTareas();
    cout<<endl;
}

void ListaEmpleados::addTarea(int cod, int area, string des){
    Empleado * empleado;
    if(!find(cod,empleado))return;
    empleado->tareas.add(area,des);
    empleado->cantidadTareas += 1;
}

bool ListaEmpleados::find(int cod, Empleado *& empleado){
    if(!inicio)return false;
    empleado = inicio;
    while(empleado){
        if(empleado->codEmpleado == cod)return true;;
        empleado = empleado->siguiente;
    }
    return false;
}

void ListaEmpleados::addTarea(int area, string des){
    if(!inicio)return;
    Empleado* empleado = min();
    empleado->tareas.add(area,des);
    empleado->cantidadTareas += 1;
}



ListaEmpleados::Empleado* ListaEmpleados::max(){
    if(!inicio)return nullptr;
    Empleado * mayor = inicio;
    Empleado * temp = inicio->siguiente;
    while(temp){
        if(mayor->cantidadTareas < temp->cantidadTareas){
            mayor = temp;
        }
        temp = temp->siguiente;
    }
    return mayor;
}

ListaEmpleados::Empleado* ListaEmpleados::min(){
    if(!inicio)return nullptr;
    Empleado * menor = inicio;
    Empleado * temp = inicio->siguiente;
    while(temp){
        if(menor->cantidadTareas > temp->cantidadTareas){
            menor = temp;
        }
        temp = temp->siguiente;
    }
    return menor;
}

void ListaEmpleados::addEmpleado(string nombre){
    Empleado * nuevo = new Empleado(tam, nombre);
    tam++;
    if(!inicio){
        inicio = nuevo;
        fin = nuevo;
    }
    else{
        fin->siguiente = nuevo;
        fin = nuevo;
    }
}

ListaEmpleados::Empleado::Empleado(){
    codEmpleado = -1;
    cantidadTareas = 0;
    nombre = nullptr;
    siguiente = nullptr;
}

ListaEmpleados::Empleado::Empleado(int cod, string nombre){
    codEmpleado = cod;
    this->nombre = nombre;
    cantidadTareas = 0;
    siguiente = nullptr;
}

ListaEmpleados::ListaEmpleados(){
    inicio = nullptr;
    tam = 0;
}

ListaEmpleados::~ListaEmpleados(){}

#endif // LISTAEMPLEADOS_H
