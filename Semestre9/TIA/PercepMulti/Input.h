#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <vector>

using namespace std;

typedef double Val;
typedef function<Val(Val)> ActivationFunc;
typedef tuple<ActivationFunc,ActivationFunc> ActsFuns;

class Input{
	public:
		Input(int n);
		Input(vector<Val> v);
		void generateInput(Input in);
		void clear(){free(vec);}
		Input(){vec = NULL;}
		void print();
		Val& operator[](int i){return vec[i];}

		Val * vec;
		int size;
};

Input::Input(int n){
	srand(time(NULL));
	size = n;
	vec = new Val[n];
	for(int i = 0; i < n; i++){
		vec[i] = rand() % 2;
	}
	vec[0] = 1;
}

Input::Input(vector<Val> v){
	size = v.size();
	this->vec = new Val[size];
	for(int i = 0; i < size; i++){
		this->vec[i] = v[i];
	}
}

void Input::generateInput(Input in){
	size = in.size - 1;
	vec = new Val[size];
	if(size > 0) memcpy(vec,&in.vec[1],sizeof(Val) * size);
	else vec = in.vec;
}

void Input::print(){
	for(int i = 0; i < size; i++){
		cout<<vec[i]<<" ";
	}
	cout<<endl<<endl;
}

#endif