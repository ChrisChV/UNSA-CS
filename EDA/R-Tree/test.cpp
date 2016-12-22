#include <iostream>
#include <vector>

using namespace std;

class hola{
public:
	int a;
};

class hola2 : public hola{
public:
	int b;
};

int main(){
	vector<hola> hh;
	hola2 h;
	h.a = 10;
	h.b = 20;
	hh.push_back(h);
	
}