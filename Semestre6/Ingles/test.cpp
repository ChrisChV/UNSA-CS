#include <iostream>
#include <map>

using namespace std;

void printMap(map<string,string> &m){
	for(auto iter = m.begin(); iter != m.end(); ++iter){
		cout<<iter->first<<endl;
	}
}

int main(){
	map<string,string> diccionario;
	diccionario["zzzz"] = "zzz";
	diccionario["casa"] = "casa";
	diccionario["mapa"] = "mapa";
	diccionario["hora"] = "hora";
	diccionario["chris"] = "chris";
	diccionario["aaaa"] = "aaaa";
	printMap(diccionario);
}