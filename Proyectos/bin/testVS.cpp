#include <iostream>
#include "VS/VS.h"
#include "Utils/Utils.h"

void printBit(CImg<int> &img){
	for(int i = 0; i < 10; i++){
		cout<<toBinary(img(i,100,0,0))<<endl;
	}
}

void print(CImg<int> &img){
	for(int i = 0; i < 10; i++){
		cout<<img(i,100,0,0)<<endl;
	}
}

int main(){
	cout<<convertToBMP("mono.bmp")<<endl;
	//CImg<int> original("mono2.jpg");
	//vector<CImg<int>> shares = VS("mono2.jpg");
	//CImg<int> res = VS(shares);

	//(original,shares[0],shares[1],shares[2],shares[3],shares[4],shares[5],res).display();

	//CImg<int> share0R0;
	//CImg<int> share0R1;
	//tie(share0R0,share0R1) = binaryVS(desR[0]);
	//desR[0].display();
	//(share0R0,share0R1).display();
	//CImg<int> res = binaryVS(share0R0,share0R1);
	//(*R,desR[0],share0R0,share0R0,res).display();





}