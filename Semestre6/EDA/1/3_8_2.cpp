#include <iostream>

using namespace std;

typedef float (*lpfnOperation)(float, float);

float Addition(float a, float b){return a + b;}
float Subtraction(float a, float b){return a - b;}
float Multiplication(float a, float b){return a * b;}
float Division(float a, float b){
	if(b == 0.0){
		throw (int)0;
	}
	return a / b;
}


void CalA(){
	float a, b;
	float c = 0;
	int opt;
	cout<<"Ingrese los operandos:"<<endl;
	cout<<"A->";
	cin>>a;
	cout<<"B->";
	cin>>b;
	cout<<"Ingrese su operacion (0:+,1:-,2:*,3:/)->";
	cin>>opt;
	try{
		switch(opt){
			case 0: c = Addition(a,b); break;
			case 1: c = Subtraction(a,b); break;
			case 2: c = Multiplication(a,b); break;
			case 3: c = Division(a,b); break;
		}
		cout<<"Resultado:"<<c<<endl;
	}
	catch(int e){
		cout<<"La division no puede ser sobre 0"<<endl;
	}

}

void CalB(){
	lpfnOperation vpf[4] = {Addition, Subtraction, Multiplication, Division};
	float a, b;
	float c = 0;
	int opt;
	cout<<"Ingrese los operandos:"<<endl;
	cout<<"A->";
	cin>>a;
	cout<<"B->";
	cin>>b;
	cout<<"Ingrese su operacion (0:+,1:-,2:*,3:/)->";
	cin>>opt;
	try{
		c = vpf[opt](a,b);
		cout<<"Resultado:"<<c<<endl;
	}
	catch(int e){
		cout<<"La division no puede ser sobre 0"<<endl;
	}


}


class CalInterface{
	public:
		static float Addition(float a, float b){return a + b;}
		static float Subtraction(float a, float b){return a - b;}
		static float Multiplication(float a, float b){return a * b;}
		static float Division(float a, float b){
			if(b == 0.0){
				throw (int)0;
			}
			return a / b;
		}		
};

int main(){
	//CalA();
	//CalB();
	
}