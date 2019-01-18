#include <iostream>
#include <cmath>

using namespace std;

typedef long double Num;

Num f(Num x){
	return pow(x,3);
}

Num reglaTrapecio(Num a, Num b, int n, Num(*f)(Num)){
	Num h = (b-a)/n;
	Num suma = 0;
	for(int i = 1; i < n; i++){
		suma += f(a + i*h);
	}
	return suma * h + (h/2.0) * (f(a) + f(b)); 
}

Num reglaSimpson1_3(Num a, Num b, int n, Num(*f)(Num)){
	if(n % 2 != 0) return -10000;
	Num h = (b-a)/n;
	Num suma1 = 0;
	Num suma2 = 0;
	for(int i = 1; i < n; i++){
		if(i % 2 == 0) suma1 += 2 * (h/3) * f(a+i*h);
		else suma2 += 4 * (h/3) * f(a+i*h);
	}
	return suma1 + suma2 + (h/3) * (f(a) + f(b));
}

int main(){
	Num (*fun)(Num) = f;
	Num a;
	Num b;
	int n;
	int flag;
	cout<<"Regla Trapecio(1) - Regra Simpson 1/3 (2)->";
	cin>>flag;
	cout<<"Ingrese su a->";
	cin>>a;
	cout<<"Ingrese su b->";
	cin>>b;
	cout<<"Ingrese su n->";
	cin>>n;
	if(flag == 1) cout<<reglaTrapecio(a,b,n,f)<<endl;
	else if(flag == 2) cout<<reglaSimpson1_3(a,b,n,f)<<endl;
}