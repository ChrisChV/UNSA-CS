#include <iostream>
#include "Hopfield.h"

using namespace std;


int main(){
	vector<Patron> patrones = {{1,1,1},{-1-1-1}};
	Hopfield h(patrones);
	run();
	Patron res = evaluate({-1,1,1});
	for(ValType v : res){
		cout<<v<<" ";
	}
	cout<<endl;
}