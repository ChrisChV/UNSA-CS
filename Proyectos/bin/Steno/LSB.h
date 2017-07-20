#ifndef LSB_H
#define LSB_H

#include <iostream>
#include <tuple>
#include "../Utils/sad.h"

using namespace std;


//string toBinary(int num);
//int sToDecimal(string num);
string getLSB(string num, int nLSB);
string getMsg(string fileMsg);
string getStringMsgFile(string fileMsg);
string getStringMSG(string msg);
tuple<string,string,string> getLSBfromImg(string fileImg, int nLSB);
string changeLSB(string pixel, string LSB, int nLSB);
void changeImg(tuple<string,string,string> LSBs, string fileImg, int nLSB, string outFile);


#endif