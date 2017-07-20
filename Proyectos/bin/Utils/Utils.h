#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <tuple>

using namespace std;

string toBinary(int num);
int sToDecimal(string num);
string convertToBMP(string file);
tuple<string,string> getExtension(string file);

#endif