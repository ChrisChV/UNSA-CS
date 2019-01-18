#include <iostream>
#include <wn.h>
#include <cstring>

using namespace std;

int main(){
	string m = "mom";
	char * s = new char[m.size() + 1];
	strcpy(s,m.c_str());
	char * a = findtheinfo(s,1,1,1);
	synsets(m)
	string res(a);	
}