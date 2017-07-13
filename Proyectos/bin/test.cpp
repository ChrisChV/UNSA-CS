#include <iostream>
#include <fstream>
#include <ctime>
#include "Steno.h"


using namespace std;

int main(){
	clock_t ini = clock();
	string msg = getMsg("tt");
	string key = hide(msg,"milla.bmp","key");
	string res = reveal("key", "outmilla.bmp");
	clock_t end = clock();
	double s = (double) (end - ini) / CLOCKS_PER_SEC;
	cout<<msg.size()<<endl;
	cout<<key<<endl;
	cout<<res<<endl;
	cout<<s<<endl;



}











