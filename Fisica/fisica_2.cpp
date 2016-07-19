#include <iostream>
#include <cmath>
 
using namespace std;

typedef long double Var;


Var funcion(Var x){
  //return 1+ (x*x*x*x) - (3 * (x * x));
  //return x*x;
  //return 3 * x;
  return 1 / (x * x + 16);
}

Var ReimanIzquierda(Var IntervaloInicial, Var IntervaloFinal, Var rectangulos){
  Var ancho = abs(IntervaloInicial-IntervaloFinal)/rectangulos;
  Var SumaRiemann = 0;
  Var contador = IntervaloInicial;
  while(contador<IntervaloFinal){
    SumaRiemann+= ancho*funcion(contador);
    cout<<SumaRiemann<<endl;
    contador+=ancho;
  }
  return SumaRiemann;
}

Var por_trapecio(Var a, Var b, Var n){
  Var h = (b-a)/n;
  Var fa = funcion(a);
  Var sumatoria = 0;
  for(int i = 1; i < n; i++){
    sumatoria += 2 * funcion(a + i*h);
  }
  return (h/2) * (fa + sumatoria + funcion(b));
}
 
int main(){
 
  //cout<<"Integral(Área): "<<ReimanIzquierda(1,10,29)<<endl;
  cout<<"Trapecio->"<<por_trapecio(0,3,6)<<endl;

  return 0;
};

