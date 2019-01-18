#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

void filtro(int n){
	string file = "Resultados6/000" + to_string(n);
	ifstream archivo(file.c_str());
	string file2 = "Resultados7/000" + to_string(n);
	ofstream res(file2.c_str());
	char linea[200];
	string palabra1 = "";
	string palabra2 = "";
	int total = 0;
	int total2 = 0;
	while(archivo.getline(linea,200)){
		cout<<total2<<endl;
		total2++;
		bool flag = true;
		palabra1.clear();
		palabra2.clear();
		for(char c : linea){
			if(c == ' ' and flag) flag = false;
			else if(c == '	' and !flag){
				bool flag1 = true;
				transform(palabra1.begin(), palabra1.end(), palabra1.begin(), ::tolower);
				transform(palabra2.begin(), palabra2.end(), palabra2.begin(), ::tolower);
				string word;
				ifstream stopwords("stopwords.txt");
				while(stopwords>>word){
					if(word == palabra1 or word == palabra2){
						flag1 = false;
						break;
					}
				}
				stopwords.close();
				if(flag1){
					res<<linea<<endl;
					total++;	
				}
				break;
			}
			else if(flag) palabra1.push_back(c);
			else if(!flag) palabra2.push_back(c);
		}
	}
	archivo.close();
	res.close();
	string fi = "Resultados7/total000" + to_string(n);
	ofstream tt(fi);
	tt<<total;
	tt.close();
}

int main(){
	for(int i = 13; i < 32; i++){
		filtro(i);
	}
}
