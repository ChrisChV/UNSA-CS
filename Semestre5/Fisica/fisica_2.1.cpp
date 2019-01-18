#include <iostream>
#include <cmath>

typedef long double numero;

using namespace std;

numero funcion(numero x){
  return x*x;
}

int main(){
  numero IntervaloInicial = 0;
  numero IntervaloFinal = 5;
  numero rectangulos = 10;
  numero ancho = abs(IntervaloInicial-IntervaloFinal)/rectangulos;

  numero SumaRiemannIzquierda = 0;
  numero contador = IntervaloInicial;
  while(contador<IntervaloFinal){
    SumaRiemannIzquierda+= ancho*funcion(contador);
    contador+=ancho;
  }

  numero SumaRiemannDerecha = 0;
  contador = IntervaloInicial+ancho;
  while(contador<=IntervaloFinal){
    SumaRiemannDerecha+= ancho*funcion(contador);
    contador+=ancho;
  }

  numero SumaRiemannMedio = 0;
  contador = IntervaloInicial+ancho/2;
  while(contador<IntervaloFinal){
    SumaRiemannMedio+= ancho*funcion(contador);
    contador+=ancho;
  }
  
  numero SumaRiemannTrapecio = 0;
  contador = IntervaloInicial;
  while(contador<IntervaloFinal){
    SumaRiemannTrapecio+= (funcion(contador)+funcion(contador+ancho))*ancho/2;
    contador+=ancho;
  }

  cout<<"Riemann(izquierda): "<<SumaRiemannIzquierda<<endl;
  cout<<"Riemann(derecha): "<<SumaRiemannDerecha<<endl;
  cout<<"Riemann(medio): "<<SumaRiemannMedio<<endl;
  cout<<"Riemann(trapecio): "<<SumaRiemannTrapecio<<endl;
  return 0;
};