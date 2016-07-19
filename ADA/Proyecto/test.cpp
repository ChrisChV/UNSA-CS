#include <iostream>
#include <vector>

using namespace std;

int main(){
	vector<int> a;
	for(int i = 0; i < 10; i++){
		a.push_back(i);
	}
	(*(a.begin() + 3)) = 100;
	for(auto iter = a.begin(); iter != a.end(); ++iter){
		cout<<*iter<<endl;
	}
}