#ifndef VS_H
#define VS_H

#include <iostream>
#include <tuple>
#include "../CImg.h"
#include "../Utils/sad.h"
#include <pthread.h>
#include <vector>

using namespace std;
using namespace cimg_library;


//string toBinary(int num);
//int sToDecimal(string num);
tuple<CImg<int>,CImg<int>,CImg<int>> getChannels(string fileImg);
tuple<CImg<int>,CImg<int>> binaryVS(CImg<int> &img);
CImg<int> binaryVS(CImg<int> &share1, CImg<int> &share2);
vector<CImg<int>> bitDescompose(CImg<int> &img);
CImg<int> bitComposition(vector<CImg<int>> &imgs);
void * threadBitDescompose(void * arg);
vector<CImg<int>> VS(string fileImg);
CImg<int> VS(vector<CImg<int>> &shares);


struct ThreadArg{
	int ini;
	int end;
	CImg<int> * img;
	vector<CImg<int>> * res;
	ThreadArg(){}
	ThreadArg(int a, int b, CImg<int> * i, vector<CImg<int>> * r){
		ini = a;
		end = b;
		img = i;
		res = r;
	}
};

#endif