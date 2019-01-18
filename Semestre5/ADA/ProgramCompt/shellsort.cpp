#include <iostream>
#include <vector>

using namespace std;

int buscar(vector<string> &esperado, string s){
	for(int i = 0; i < esperado.size(); i++){
		if(esperado[i] == s) return i;
	}
}

bool bienPosicionado(vector<string> &original, vector<string> &esperado, int a){
	string s = original[a];
	int b = buscar(esperado, s) + 1;
	for(; b < esperado.size(); b++){
		string ss = esperado[b];
		bool flag = false;
		for(int aa = a+1; aa < original.size();aa++){
			if(original[aa] == ss){
				flag = true;
				break;
			}
		}
		if(!flag) return false;
	}
	return true;
}

int main(){
	int n;
	cin>>n;
	for(int i = 0; i < n; i++){
		int n2;
		vector<string> original;
		vector<string> esperado;
		vector<string> res;
		cin>>n2;		
		char temp[10];
		cin.getline(temp,10);
		for(int j = 0; j < n2; j++){
			char s[800];
			cin.getline(s,800);
			string ss(s);
			original.push_back(ss);
		}
		//char temp[10];
		//cin.getline(temp,10);
		for(int j = 0; j < n2; j++){
			char s[800];
			cin.getline(s,800);
			string ss(s);
			esperado.push_back(ss);
		}
		while(original != esperado){
			for(int j = 0; j < n2; j++){
				if(!bienPosicionado(original,esperado,j)){
					string temp = original[j];
					original.erase(original.begin()+j);
					original.insert(original.begin(),temp);
					res.push_back(temp);
				}
			}
		}
		for(int j = 0; j < res.size(); j++){
			cout<<res[j]<<endl;
		}
		cout<<endl;
	}
}w