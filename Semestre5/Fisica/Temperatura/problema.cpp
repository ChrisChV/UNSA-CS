#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

typedef long double Num;

Num mf = 30;
Num mc = 20;
Num Tf = -15;
Num Tc = 20;
Num Ch = 0.5;
Num La = 80;
Num Ca = 1;

Num Q1(Num T, int r){
	if(r == 2) return Ch * mf * (0 - Tf) + La * mf + Ca * mf * (T - 0) + Ca * mc * (T - Tc); //2
	return Ch * mf * (T - Tf) + Ca * mc * (0 - Tc) + (-La * mc) + Ch * mc + (T - 0); //1
}

Num Q2(Num m, int r){
	if(r == 4) return Ch * mf * (0-Tf) + La * (m) + Ca * (mc) * (0-Tc); //4
	else if(r == 3) return Ch * (mf) * (0 - Tf) + Ca * mc * (0 - Tc) - La * (m); // 3
	return Ch * (mf+m) * (0 - Tf) + La * (mf - m) + Ca * (mc+m) * (0 - Tc) - La * (mc - m); //5
}

int main(){
	Num Qr;
	int r;
	
	while(true){
	ofstream archivo("gra.dat");
	archivo<<"plot '-' using 1:2"<<endl;
	cout<<"Escriba su ruta->";
	cin>>r;
	if(r == 1){
		for(float T = -15; T <= 0; T+=0.1){ //1	
			Qr = Q1(T,1);
			archivo<<T<<" "<<Qr<<endl;
		}
		for(float T = 0; T <= 20; T+=0.1){ //2	
			Qr = Q1(T,2);
			archivo<<T<<" "<<Qr<<endl;
		}
	}
	else if(r == 2){
		for(float T = 0; T <= 20; T+=0.1){ //2	
			Qr = Q1(T,r);
			archivo<<T<<" "<<Qr<<endl;
		}
	}
	else if(r == 3){
		for(float m = 0; m <= mc; m+=0.01){
			Qr = Q2(m,r);
			archivo<<m<<" "<<Qr<<endl;
		}
	}
	else if(r == 4){
		for(float m = 0; m <=mf; m+=0.01){
			Qr = Q2(m,r);
			archivo<<m<<" "<<Qr<<endl;	
		}
	}
	else if(r == 5){
		for(float m = 0; m <= 20; m+= 0.01){
			Qr = Q2(m,r);
			archivo<<m<<" "<<Qr<<endl;
		}
	}
	//for(float T = 0; T <= 20; T+=0.1){ //1
	//for(float T =-15; T <= 0; T+=0.1){ //2
	//for(float T = Tf ; T <= Tc; T+=0.1){ //3
	//for(float m = 0; m <=mf; m+=0.01){ //4
	//for(float m = 0; m <= mc; m+=0.01){
	archivo.close();
	system("gnuplot -p < \"gra.dat\"");
	}
	
}