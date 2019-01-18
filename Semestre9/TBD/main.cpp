#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "Utils/FileOp.h"

using namespace std;

int main(){
	auto names = getBdNames("names");
	auto vals = getBdVals("vals", names.size());

	ofstream outFile("outVals");

	for(int i = 0; i < vals.size(); i++){
		for(int j = 0; j < vals[i].size(); j++){
			if(vals[i][j] == NULL_VAL){
				outFile<<"-"<<" ";
				continue;	
			} 
			outFile<<vals[i][j]<<" ";
		}
		outFile<<endl;
	}

	outFile.close();
}