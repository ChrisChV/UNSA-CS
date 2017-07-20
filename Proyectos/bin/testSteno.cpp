#include <iostream>
#include <fstream>
#include <ctime>
#include "Steno/StenoOrig.h"


using namespace std;

int main(){
	clock_t ini = clock();
	string msg = getMsg("imgtest");
	string key = hide(msg,"mono2.jpg","key");
	string res = reveal("key", "outmono2.jpg");
	clock_t end = clock();
	double s = (double) (end - ini) / CLOCKS_PER_SEC;
	cout<<msg.size()<<endl;
	cout<<key<<endl;
	cout<<res<<endl;
	cout<<s<<endl;
}











