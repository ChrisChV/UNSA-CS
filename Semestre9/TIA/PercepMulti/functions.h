#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <tuple>
#include <functional>
#include <cmath>
#include "Input.h"

using namespace std;

Val _signoide(Val val){
	return  1.0 / (1.0 + exp(-val));
}

Val _signoideDerivate(Val val){
	return (1 - val)  * val;
}

Val _tanH(Val val){
	return  (exp(val) - exp(-val)) / (exp(val) + exp(-val));
}

Val _tanHDerivate(Val val){
	return 1 - pow(_tanH(val),2);
}

Val _relu(Val val){
	if(val < 0) return 0;
	else return val;
}

Val _reluDerivate(Val val){
	if(val < 0) return 0;
	else return 1;
}

ActsFuns SignoideFunc = make_tuple(_signoide,_signoideDerivate);
ActsFuns TangenteHFunc = make_tuple(_tanH, _tanHDerivate);
ActsFuns ReluFunc = make_tuple(_relu, _reluDerivate);


#endif
