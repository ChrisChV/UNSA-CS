#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include "../VS/VS.h"
#include "../Steno/Steno.h"


using namespace std;

string crifrar_ocultar(string img, string stegoImg1, string stegoImg2, string keyFile){
	img = convertToBMP(img);
	stegoImg1 = convertToBMP(stegoImg1);
	stegoImg2 = convertToBMP(stegoImg2);
	vector<CImg<int>> shares = VS(img);
	cout<<"Shares generados correctamente"<<endl;
	return hide(shares,stegoImg1,stegoImg2,keyFile);
}

CImg<int> revelar_descrifrar(string key){
	cout<<"Inicio"<<endl;
	vector<CImg<int>> shares = reveal(key);
	(shares[0],shares[1],shares[2],shares[3],shares[4],shares[5]).display();
	cout<<"Shares revelados correctamente"<<endl;
	return VS(shares);
}


#endif