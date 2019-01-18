#include <iostream>

using namespace std;

auto fun(int i){
	if(i == 0) return "hola mundo";
	else return "naco";
}

int main(){
	int i = 0;
	string s = fun(i);
	cout<<s<<endl;
}