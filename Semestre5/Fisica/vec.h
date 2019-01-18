#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <cmath>
#include "integral.h"

typedef float valor;

using namespace std;

numero funcion1(numero x){return 2*x;}
numero funcion2(numero x){return -1 * x*x*x;}
numero funcion3(numero x){return (-1 *x) + 1;}

class Vector3 {
  public:
    Vector3();
    Vector3(valor,valor,valor);
    valor getX(){return this->x;}
    valor getY(){return this->y;}
    valor getZ(){return this->z;}
    void operator*(Vector3); // Producto Escalar
    void operator+(Vector3); // Suma
    void operator-(Vector3); // Resta
    Vector3 operator*=(Vector3);// Producto Vectorial entre dos vectores
    Vector3 operator*=(valor); //Producto Vectorial entre vector y un número
    void integral(numero t);
    void modulo();
    void print();

  private:
    valor x;
    valor y;
    valor z;
};


Vector3::Vector3()
{x=y=z=0;}

Vector3::Vector3(valor x, valor y, valor z)
{
  this->x=x;
  this->y=y;
  this->z=z;
}

void Vector3::integral(numero t){
  numero (*fun1)(numero) = funcion1;
  numero (*fun2)(numero) = funcion2;
  numero (*fun3)(numero) = funcion3;
  cout<<"Integral: ("<<Riemann_Medio(0,t,500,fun1)<<','<<Riemann_Medio(0,t,500,fun2)<<','<<Riemann_Medio(0,t,500,fun3)<<")"<<endl;
}

void Vector3::operator*(Vector3 v2)
{
  cout<<"Producto Escalar: "<<this->x*v2.getX()+this->y*v2.getY()+this->z*v2.getZ()<<'\n';
}

void Vector3::operator+(Vector3 v2)
{
  cout<<"Suma: "<<'('<<this->x+v2.getX()<<','<<this->y+v2.getY()<<','<<this->z+v2.getZ()<<')'<<'\n';
}

void Vector3::operator-(Vector3 v2)
{
  cout<<"Resta: "<<'('<<this->x-v2.getX()<<','<<this->y-v2.getY()<<','<<this->z-v2.getZ()<<')'<<'\n';
}

Vector3 Vector3::operator*=(Vector3 v2)
{
  return Vector3(this->y*v2.getZ()-this->z*v2.getY(),this->z*v2.getX()-this->x*v2.getZ(),this->x*v2.getY()-this->y*v2.getX());
}

Vector3 Vector3::operator*=(valor t){
  return Vector3(x*t,y*t,z*t);
}

void Vector3::modulo()
{
  cout<<"Módulo: "<< pow(pow(this->x,2)+pow(this->y,2)+pow(this->z,2),0.5)<<'\n';
}

void Vector3::print()
{
  cout<<"Vector: "<<'('<<this->x<<','<<this->y<<','<<this->z<<')'<<'\n';
}

#endif