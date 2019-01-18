#include <iostream>
#include <cmath>

using namespace std;

int cifras_sig(float xs, float x){
	float v = (abs(x - xs)) / abs(x);
	int i = 0;
	for(; v < 0.5 * pow(10,-1*i); i++)
	{
		cout<<v<<"<"<<0.5 * pow(10,-1*i)<<endl;
	}
	return i - 1;
}

int main(){
	float xs;
	float x;
	cout<<"Ingrese xs->"<<endl;
	cin>>xs;
	cout<<"Ingrese x->"<<endl;
	cin>>x;
	cout<<cifras_sig(xs,x)<<endl;
}