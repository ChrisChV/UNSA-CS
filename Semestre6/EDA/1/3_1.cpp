#include <iostream>

using namespace std;

int gVect[100];
int gnCount = 0;

int *gpVect = NULL;
int gnpCount = 0;
int gnpMax = 0;

void printVects(){
	for(int i = 0; i < gnCount; i++){
		cout<<gVect[i]<<"->";
	}
	cout<<endl;
	for(int i = 0; i < gnpCount; i++){
		cout<<gpVect[i]<<"->";
	}
	cout<<endl;
}

void Insert(int elem){
	if(gnCount < 100){
		gVect[gnCount++] = elem;
	}
}

void Resize(){
	const int delta = 2;
	gpVect = (int *) realloc(gpVect, sizeof(int) * (gnpMax + delta));
	gnpMax += delta;
}

void InsertP(int elem){
	if(gnpCount == gnpMax){
		Resize();
	}
	gpVect[gnpCount++] = elem;
}

int main(){
	for(int i = 0; i < 20; i++){
		Insert(i);
		InsertP(i);
	}
	printVects();
}