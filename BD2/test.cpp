#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void ff(string * s, char c){
	string a = "v";
	for(int i = 0; i < 10; i++){
		string b;
		b.push_back(c);
		a = a + b;
	}
	*s= a;
}

int main(){
	string s = "hola";
	s.pop_back();
	cout<<s<<endl;
}