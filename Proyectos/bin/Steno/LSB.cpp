#include <iostream>
#include <tuple>
#include <fstream>
#include <cmath>
#include <vector>
#include "../CImg.h"
#include "../Utils/Utils.h"
#include "LSB.h"

using namespace cimg_library;
using namespace std;

/*
string toBinary(int num){
	string res = "";
	int temp = 0;
	for(int i = 0; i < 8; i++){
		temp = pow(2,7-i);
		if(num >= temp){
			num -= temp;
			res.push_back('1');
		}
		else res.push_back('0');
	}
	return res;
}

int sToDecimal(string num){
	int res = 0;
	for(int i = 0; i < num.size(); i++){
		if(num[i] == '1') res += pow(2,num.size() - 1 - i);
	}
	return res;
}
*/

string getLSB(string num, int nLSB){
	string res = "";
	for(int i = 0; i < nLSB; i++){
		res.push_back(num[BITS_PER_PIXEL - 1 - (nLSB - i - 1)]);
	}
	return res;
}

string getMsg(string fileMsg){
	ifstream archivo(fileMsg.c_str());
	string res = "";
	char temp = 0;
	while(archivo.get(temp)){
		res.push_back(temp);
	}
	archivo.close();
	return res;
}

string getStringMsgFile(string fileMsg){
	string msg = getMsg(fileMsg);
	string res = "";
	for(char c : msg){
		res += toBinary((int) c);
	}
	return res;
}

string getStringMSG(string msg){
	string res = "";
	for(char c : msg){

		res += toBinary((int) c);
	}
	return res;	
}

string getBinaryMSG(CImg<int> &img){
	string res = "";
	for(int i = 0; i < img.height(); i++){
		for(int j = 0; j < img.width(); j++){
			res += toBinary(img(j,i,0,0));
		}
	}
	return res;		
}

tuple<string,string,string> getLSBfromImg(string fileImg, int nLSB){
	CImg<int> img(fileImg.c_str());
	string res0 = "";
	string res1 = "";
	string res2 = "";
	for(int i = 0; i < img.height(); i++){
		for(int j = 0; j < img.width(); j++){
			res0 += getLSB(toBinary(img(j,i,0)),nLSB);
			res1 += getLSB(toBinary(img(j,i,1)),nLSB);
			res2 += getLSB(toBinary(img(j,i,2)),nLSB);
		}
	}
	return make_tuple(res0,res1,res2);
}

string changeLSB(string pixel, string LSB, int nLSB){
	for(int i = 0; i < nLSB; i++){
		//pixel[BITS_PER_PIXEL - 1 - (nLSB - i - 1)] = LSB[nLSB - i - 1];
		pixel[BITS_PER_PIXEL - 1 - (nLSB - i - 1)] = LSB[i];
	}
	return pixel;
}

void changeImg(tuple<string,string,string> LSBs, string fileImg, int nLSB, string outImg){
	CImg<int> img(fileImg.c_str());
	vector<string> LSBimg;
	LSBimg.push_back(get<0>(LSBs));
	LSBimg.push_back(get<1>(LSBs));
	LSBimg.push_back(get<2>(LSBs));
	string LSB = "";
	string pixel = "";
	vector<int> actual;
	for(int i = 0; i < NUMBER_CHANNELS; i++){
		actual.push_back(0);
	}
	for(int i = 0; i < img.height(); i++){
		for(int j = 0; j < img.width(); j++){
			for(int k = 0; k < NUMBER_CHANNELS; k++){
				LSB.clear();
				pixel.clear();
				for(int l = 0; l < nLSB; l++){
					LSB.push_back(LSBimg[k][actual[k]]);
					actual[k]++;
				}
				pixel = toBinary(img(j,i,k));
				pixel = changeLSB(pixel,LSB,nLSB);
				img(j,i,k) = sToDecimal(pixel);
			}
		}
	}
	img.save(outImg.c_str());
}

