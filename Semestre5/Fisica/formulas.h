#ifndef FORMULAS_H
#define FORMULAS_H

#include <iostream>
#include <cmath>
#include <tuple>

#define PI 3.14159265
#define GRAVEDAD_TIERRA 9.78

typedef long double Number;


using namespace std;

// Caida Libre and Moviemiento Parabolico

Number VelocidadInicialEnY(Number velocidadInicial, Number angulo){
	return velocidadInicial * sin(angulo*PI/180);
}

Number VelocidadInicialEnX(Number velocidadInicial, Number angulo){
	return velocidadInicial * cos(angulo*PI/180);
}

Number VelocidadEnCaida(Number aceleracion, Number velocidadInicial, Number tiempo) {
	return -1 * aceleracion * tiempo + velocidadInicial;
}

Number VelocidadEnY(Number aceleracion, Number velocidadInicial, Number tiempo, Number angulo){
	return VelocidadEnCaida(aceleracion, VelocidadInicialEnY(velocidadInicial,angulo), tiempo);
}

Number PosicionEnCaida(Number aceleracion, Number velocidadInicial, Number tiempo, Number posicionInicial){
	return -1 * 0.5 * aceleracion * tiempo * tiempo + velocidadInicial * tiempo + posicionInicial;
}

Number PosicionEnY(Number aceleracion, Number velocidadInicial, Number tiempo, Number angulo, Number posicionInicial){
	return PosicionEnCaida(aceleracion,VelocidadInicialEnY(velocidadInicial,angulo),tiempo,posicionInicial);
}

Number TiempoSubida(Number aceleracion, Number velocidadInicial){
	return velocidadInicial / aceleracion;
}

Number TiempoVuelo(Number aceleracion, Number velocidadInicial){
	return 2 * TiempoSubida(aceleracion, velocidadInicial);
}

Number Elevacion(Number aceleracion, Number velocidadInicial){
	return 0.5 * ((velocidadInicial * velocidadInicial)/aceleracion);
}

Number Alcance(Number aceleracion, Number velocidadInicialEnX, Number velocidadInicialEnY){
	return (velocidadInicialEnX * velocidadInicialEnY) / aceleracion;
}

Number PosicionEnX(Number velocidadInicial, Number tiempo, Number angulo, Number posicionInicial){
	return VelocidadInicialEnX(velocidadInicial,angulo) * tiempo + posicionInicial;
}

Number Elevacion(Number aceleracion, Number velocidadInicial, Number angulo){
	return 0.5 * (velocidadInicial * velocidadInicial) / aceleracion * pow(sin(angulo*PI/180),2);
}

Number AlcanceConAngulo(Number aceleracion, Number velocidadInicial, Number angulo){
	return (velocidadInicial * velocidadInicial) / aceleracion * sin(2*angulo*PI/180);
}

Number TiempoSubida(Number aceleracion, Number velocidadInicial, Number angulo){
	return (velocidadInicial * sin(angulo*PI/180) / aceleracion);
}

Number TiempoVuelo(Number aceleracion, Number velocidadInicial, Number angulo){
	return 2 * TiempoSubida(aceleracion,velocidadInicial,angulo);
}

tuple<Number,Number> HallarAngulos(Number aceleracion, Number velocidadInicial, Number posicion){
	Number t1 = (aceleracion * posicion) / (velocidadInicial * velocidadInicial);
	Number angulo1 = (asin(t1) * 180 / PI) / 2;
	return make_tuple(angulo1,90-angulo1);
}

#endif