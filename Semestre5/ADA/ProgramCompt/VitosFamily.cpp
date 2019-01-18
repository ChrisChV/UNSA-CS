#include <iostream>
#include <vector>

using namespace std;

int main(){
	int n;
	cin>>n;
	for(int i = 0; i < n; i++){
		int n2;
		cin>>n2;
		vector<int> casitas;
		for(int j = 0; j < n2; j++){
			int e;
			cin>>e;
			casitas.push_back(e);
		}
		int menor = -1;
		for(auto iter = casitas.begin(); iter != casitas.end(); ++iter){
			int s = 0;
			for(auto iter2 = casitas.begin(); iter2 != casitas.end(); ++iter2){
				if(iter != iter2){
					s += abs(*iter - *iter2);
				}
			}
			if(menor == -1 or menor > s){
				menor = s;
			}
		}
		cout<<menor<<endl;
	}
}