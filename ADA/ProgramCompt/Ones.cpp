#include <iostream>
#include <cmath>

using namespace std;

int main(){
	int n;
	while(cin>>n){
		int i = 1;
		int s = 1;
		while(s%n != 0){
			s= (s*10+1)%n;
			i++;
		}
		cout<<i<<endl;
	}
}