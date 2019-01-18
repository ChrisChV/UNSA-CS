#include <iostream>
#include <vector>
#include <map>

using namespace std;

vector<int*> vectorNull(int n){
	vector<int*> res;
	for(int i = 0; i < n; i++){
		res.push_back(new int(0));
	}
	return res;
}

int main(){
	map<char,vector<int>> star;
	star['A'] = vectorNull(11);
	star['B'] = vectorNull(9);
	star['C'] = vectorNull(9);
	star['D'] = vectorNull(11);
	star['E'] = vectorNull(11);
	star['F'] = vectorNull(9);
	star['G'] = vectorNull(9);
	star['H'] = vectorNull(11);
	star['I'] = vectorNull(11);
	star['J'] = vectorNull(9);
	star['K'] = vectorNull(9);
	star['L'] = vectorNull(11);
	

	map<char,int> in;
	for(int i = 0; i < 12; i++){
		int a;
		cin>>a;
		in[65+i] = a;
	}
	
}