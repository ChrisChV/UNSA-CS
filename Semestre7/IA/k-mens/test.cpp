#include <iostream>
#include <cstdlib>
#include <cstring>
#include <array>

using namespace std;

typedef float Value;

struct Points{
	Value ** m;
	int fil;
	int col;
	Points(){
		m = nullptr;
		fil = 0;
		col = 0;
	}
	Points(int fil, int col){
		this->fil = fil;
		this->col = col;
		m = new Value *[fil];
		for(int i = 0; i < fil; i++) m[i] = new Value[col];
	}
	Value *& operator[](int i){ return m[i];}
};

int main(){
	Points a(3,3);
	a[0][0] = 1;
	Points b(3,3);
	b = a;

	cout<<a[0][0]<<endl;
	b.m[0][0] = 10;
	cout<<a[0][0]<<endl;
}