#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool ss(int a, int b){
	return a < b;
}

int main(){
	vector<int> cc = {4,12,7,3,9,2,1};
	for(auto iter = cc.begin(); iter != cc.end(); ++iter){
		if(*iter % 2 == 0){
			cc.erase(iter);
			--iter;
		}
	}
	for(int a : cc){
		cout<<a<<endl;
	}
}