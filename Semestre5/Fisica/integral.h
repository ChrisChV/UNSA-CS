#include <iostream>
#include <cmath>

typedef long double numero;

using namespace std;

numero Riemann_Medio(numero IntervaloInicial, numero IntervaloFinal, numero rectangulos, numero(*funcion)(numero x)){
  numero ancho = abs(IntervaloInicial-IntervaloFinal)/rectangulos;
  numero SumaRiemannMedio = 0;
  numero contador = IntervaloInicial+ancho/2;
  while(contador<IntervaloFinal){
    SumaRiemannMedio+= ancho*funcion(contador);
    contador+=ancho;
  }
  //cout<<"Riemann(medio): "<<SumaRiemannMedio<<endl;
  return SumaRiemannMedio;
}
