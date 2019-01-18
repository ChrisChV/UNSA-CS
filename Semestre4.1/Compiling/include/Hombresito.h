#ifndef HOMBRESITO_H
#define HOMBRESITO_H
#include <iostream>>

using namespace std;

class Hombresito
{
    public:
        Hombresito();
        Hombresito(int, bool, char);
        void print();
        void setHermano(int);
        void setPosActual(int);
        virtual ~Hombresito();
    protected:
    private:
        int dni;
        bool sexo;
        char name;
        int hermano;
        int posActual;
};

void Hombresito::setPosActual(int pos){
    posActual = pos;
}

void Hombresito::setHermano(int h){
    this->hermano = h;
}

void Hombresito::print(){
    cout<<"NAME->"<<name<<endl;
    cout<<"DNI->"<<dni<<endl;
    cout<<"SEXO->"<<sexo<<endl;
    cout<<"Pos->"<<posActual<<endl;
}

Hombresito::Hombresito(int dni, bool sexo, char name){
    this->dni = dni;
    this->sexo = sexo;
    this->name = name;
    this->posActual = 0;
}

Hombresito::~Hombresito(){

}

Hombresito::Hombresito(){
    posActual = 0;
}

#endif // HOMBRESITO_H
