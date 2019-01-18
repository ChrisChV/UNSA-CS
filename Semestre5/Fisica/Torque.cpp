#include <isotream>
#include <fstream>
#include <cmath>
#include "vec.h"

typedef float Number;

void Torque(int n , int tfin){
	Vector3 v(0,0,0);
	valor m = 0.5;
	Vector3 p = v *= m;
	Vector3 r(0,0,0);
	Vector3 L = r *=p;
	Number dt = tfin/n;
	Number cN = 0;
	Number t = 0;
	for(int i = 0; i < n; i++){
		t = i * dt;
		L = L + 
		if(i==cN or i == n){
			cN = n/200;
		}
	}
}

int main(){

}