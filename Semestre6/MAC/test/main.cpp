#include <iostream>
#include "OperacionesMatriz.h"

int main(){
	Matriz m = {{1,2},{3,4},{5,6}};
	cout<<frobenius(m)<<endl;
}