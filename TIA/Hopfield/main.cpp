#include <iostream>
#include "Hopfield.h"

using namespace std;


int main(){
	/*
	vector<Patron> patrones = {{1,1,1},{-1-1-1}};
	Hopfield h(patrones);
	h.run();
	Patron res = h.evaluate({1,-1,1});
	for(ValType v : res){
		cout<<v<<" ";
	}
	cout<<endl;
	*/

	Hopfield h("training", 784, 100);		
	h.run();
	auto res = h.evaluate("test",784, 10);
	for(Patron p : res){
		h.printPatron(p);
	}
}