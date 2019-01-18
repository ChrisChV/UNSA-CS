#include <iostream>


using namespace std;

int main(){
	int n = 1000000;
	int sum = 0;
	for(int i = 0; i <= n; i++){
		sum += i;
	}
	cout<<sum<<endl;
}