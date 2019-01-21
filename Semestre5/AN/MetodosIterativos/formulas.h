#ifndef FORMULAS_H
#define FORMULAS_H

#include <iostream>
#include <cmath>

using namespace std;

typedef long double Number;

Number ErrorRelativo(Number x, Number xs){
	return abs(xs - x) / abs(x);
}

Number ErrorAbsoluto(Number x, Number xs){
	return abs(xs - x);
}

#endif