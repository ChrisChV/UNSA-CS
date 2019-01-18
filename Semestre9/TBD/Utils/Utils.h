#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <vector>
#include <typeinfo>
#include <tuple>
#include <map>
#include <fstream>
#include <unistd.h>

using namespace std;

typedef double ValType;
typedef int IndexType;
typedef double PercentageType;
typedef vector<ValType> ValVec;
typedef vector<string> Registro;

#define NULL_VAL -1
#define ZERO_VAL 0
#define INFINITO_NEGATIVO -20000

typedef map<IndexType,map<IndexType,ValType>> SimilarityMatrix;
typedef map<string, ValType>  SimilarityVector;
typedef map<int, ValType>  SimilarityMovieLensVector;
typedef map<int, tuple<ValType,float>>  MovieLensVectorDesviacion;


void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}



ValType searchInSimilarityMatrix(SimilarityMatrix & matrix, IndexType i, IndexType j){
	auto s_map = matrix.find(i);
	if(s_map == matrix.end()){
		s_map = matrix.find(j);
		if(s_map == matrix.end()) return INFINITO_NEGATIVO;
		swap(i,j);
	}
	auto findRes = s_map->second.find(j);
	if(findRes == s_map->second.end()){
		s_map = matrix.find(j);
		if(s_map == matrix.end()) return INFINITO_NEGATIVO;
		findRes = s_map->second.find(i);
		if(findRes == s_map->second.end()) return INFINITO_NEGATIVO;
	}
	return findRes->second;
}

ValType stoVT(string number){
	return stof(number);
}

void printValVec(ValVec & x){
	for(ValType val : x){
		cout<<val<<",";
	}
	cout<<endl;
}

vector<string> splitString(string str, char delimit){
	vector<string> res;
	string word = "";
	for(char c : str){
		if(c == delimit){
			res.push_back(word);
			word.clear();
		}
		else word.push_back(c);
	}
	res.push_back(word);
	return res;
}



ValType dotProduct(ValVec & x, ValVec & y){
	if(x.size() != y.size()) return 0;
	ValType res = 0;
	for(int i = 0; i < x.size(); i++){
		if(x[i] == NULL_VAL or y[i] == NULL_VAL) continue;
		res += x[i] * y[i];
	}
	return res;
}

ValType sumatoria(ValVec & x){
	ValType res = 0;
	for(ValType val : x){
		if(val == NULL_VAL) continue;
		res += val;
	}
	return res;
}

ValVec getPromedios(vector<ValVec> & vals){
	ValVec res;
	ValType n = 0;
	ValType sum = 0;
	for(int i = 0; i < vals[0].size(); i++){
		for(int j = 0; j < vals.size(); j++){
			if(vals[j][i] == NULL_VAL) continue;
			n++;
			sum += vals[j][i];
		}
		res.push_back(sum / n);
		n = 0;
		sum = 0;
	}
	return res;
}

/*
map<string,ValType>  getPromedios(map<string,map<int,ValType>> &vals){
	map<string,ValType> res;
	ValType n = 0;
	ValType sum = 0;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		n = 0;
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2, n++){
			sum += iter2->second;
		}
		res[iter->first] = sum / n;
	}
	return res;
}
*/

ValVec getPromedios(vector<map<string, ValType>> &vals){
	ValVec res = ValVec(vals.size(), NULL_VAL);
	ValType n = 0;
	ValType sum = 0;
	for(int i = 0; i < vals.size(); i++){
		n = 0;
		sum = 0;
		for(auto iter = vals[i].begin(); iter != vals[i].end(); ++iter, n++){
			sum += iter->second;
		}
		if(n == 0) res[i] = NULL_VAL;
		else res[i] = sum / n;
	}
	return res;
}

ValVec getPromedios(vector<map<int, ValType>> &vals){
	ValVec res = ValVec(vals.size(), NULL_VAL);
	ValType n = 0;
	ValType sum = 0;
	for(int i = 0; i < vals.size(); i++){
		n = 0;
		sum = 0;
		for(auto iter = vals[i].begin(); iter != vals[i].end(); ++iter, n++){
			sum += iter->second;
		}
		if(n == 0) res[i] = NULL_VAL;
		else res[i] = sum / n;
	}
	return res;
}

vector<ValVec> normalizate(vector<ValVec> & vals){
	vector<ValVec> res;
	vector<ValType> mins;
	vector<ValType> maxs;
	ValType tempMax = NULL_VAL;
	ValType tempMin = NULL_VAL;
	ValType actualVal = 0;
	for(int i = 0; i < vals[0].size(); i++){
		for(int j = 0; j < vals.size(); j++){
			actualVal = vals[j][i];
			if(actualVal == NULL_VAL) continue;
			if(tempMin == NULL_VAL  or tempMin > actualVal) tempMin = actualVal;
			if(tempMax == NULL_VAL or tempMax < actualVal) tempMax = actualVal;
		}
		mins.push_back(tempMin);
		maxs.push_back(tempMax);
		tempMin = NULL_VAL;
		tempMax = NULL_VAL;
	}

	res = vector<ValVec>(vals.size());
	for(int i = 0; i < vals.size(); i++){
		res[i] = ValVec(vals[0].size());	
	} 

	for(int i = 0; i < vals[0].size(); i++){
		for(int j = 0; j < vals.size(); j++){
			if(vals[j][i] == NULL_VAL){
				res[j][i] = INFINITO_NEGATIVO;
				continue;	
			} 
			res[j][i] = (2 * (vals[j][i] - mins[i]) - (maxs[i] - mins[i])) / (maxs[i] - mins[i]);
		}
	}
	return res;
}

vector<map<string, ValType>> normalizate(vector<map<string, ValType>> &vals){
	vector<map<string, ValType>> res(vals.size());
	vector<ValType> mins;
	vector<ValType> maxs;
	ValType tempMax = NULL_VAL;
	ValType tempMin = NULL_VAL;
	ValType actualVal = 0;
	for(int i = 0; i < vals.size(); i++){
		for(auto iter = vals[i].begin(); iter != vals[i].end(); ++iter){
			if(tempMin == NULL_VAL or tempMin > iter->second) tempMin = iter->second;
			if(tempMax == NULL_VAL or tempMax < iter->second) tempMax = iter->second;
		}
		mins.push_back(tempMin);
		maxs.push_back(tempMax);
		tempMin = NULL_VAL;
		tempMax = NULL_VAL;
	}
	for(int i = 0; i < vals.size(); i++){
		for(auto iter = vals[i].begin(); iter != vals[i].end(); ++iter){
			res[i][iter->first] = (2 * (iter->second - mins[i]) - (maxs[i] - mins[i])) / (maxs[i] - mins[i]);
		}
	}
	return res;
}

vector<map<int, ValType>> normalizate(vector<map<int, ValType>> &vals){
	vector<map<int, ValType>> res(vals.size());
	vector<ValType> mins;
	vector<ValType> maxs;
	ValType tempMax = NULL_VAL;
	ValType tempMin = NULL_VAL;
	for(int i = 0; i < vals.size(); i++){
		if(vals[i].size() == 0) continue;
		for(auto iter = vals[i].begin(); iter != vals[i].end(); ++iter){
			if(tempMin == NULL_VAL or tempMin > iter->second) tempMin = iter->second;
			if(tempMax == NULL_VAL or tempMax < iter->second) tempMax = iter->second;
		}
		mins.push_back(tempMin);
		maxs.push_back(tempMax);
		tempMin = NULL_VAL;
		tempMax = NULL_VAL;
	}
	for(int i = 0; i < vals.size(); i++){
		for(auto iter = vals[i].begin(); iter != vals[i].end(); ++iter){
			if(maxs[i] - mins[i] == 0) res[i][iter->first] = (2 * (iter->second - mins[i]) - (maxs[i] - mins[i]));
			else res[i][iter->first] = (2 * (iter->second - mins[i]) - (maxs[i] - mins[i])) / (maxs[i] - mins[i]);
		}
	}
	return res;
}

ValType desnormalize(vector<ValVec> & vals, ValType val, IndexType userIndex){
	ValType tempMax = NULL_VAL;
	ValType tempMin = NULL_VAL;
	ValType actualVal = 0;
	for(int i = 0; i < vals.size(); i++){
		actualVal = vals[i][userIndex];
		if(actualVal == NULL_VAL) continue;
		if(tempMin == NULL_VAL or tempMin > actualVal) tempMin = actualVal;
		if(tempMax == NULL_VAL or tempMax < actualVal) tempMax = actualVal;
	}
	return 0.5 * (val + 1) * (tempMax - tempMin) + tempMin;
}

ValType desnormalize(vector<map<string, ValType>> &vals, ValType val, IndexType userIndex){
	ValType tempMax = NULL_VAL;
	ValType tempMin = NULL_VAL;
	for(auto iter = vals[userIndex].begin(); iter != vals[userIndex].end(); ++iter){
		if(tempMin == NULL_VAL or tempMin > iter->second) tempMin = iter->second;
		if(tempMax == NULL_VAL or tempMax < iter->second) tempMax = iter->second;
	}
	return 0.5 * (val + 1) * (tempMax - tempMin) + tempMin;
}

ValType desnormalize(vector<map<int, ValType>> &vals, ValType val, IndexType userIndex){
	ValType tempMax = NULL_VAL;
	ValType tempMin = NULL_VAL;
	for(auto iter = vals[userIndex].begin(); iter != vals[userIndex].end(); ++iter){
		if(tempMin == NULL_VAL or tempMin > iter->second) tempMin = iter->second;
		if(tempMax == NULL_VAL or tempMax < iter->second) tempMax = iter->second;
	}
	return 0.5 * (val + 1) * (tempMax - tempMin) + tempMin;
}

ValType sumatoriaCuadratica(ValVec & x){
	ValType res = 0;
	for(ValType val : x){
		if(val == NULL_VAL) continue;
		res += pow(val, 2);
	}
	return res;
}

ValType vectorModule(ValVec & x){
	ValType res = 0;
	for(ValType val : x){
		res += pow(val, 2);
	}
	return (ValType) sqrt(res);
}

tuple<ValVec,ValVec> deleteNulls(ValVec & x, ValVec y){
	ValVec resX;
	ValVec resY;
	for(int i = 0; i < x.size(); i++){
		if(x[i] != NULL_VAL and y[i] != NULL_VAL){
			resX.push_back(x[i]);
			resY.push_back(y[i]);
		} 
	}
	return make_tuple(resX,resY);
}

void printRegistro(Registro reg){
	for(string s : reg){
		cout<<s<<" ";
	}
	cout<<endl;
}


template<class T>
T sToNum(string str){
	return str;
}

template<>
int sToNum<int>(string str){
	return stoi(str);
}

template<>
float sToNum<float>(string str){
	return stof(str);
}

template<>
double sToNum<double>(string str){
	return stod(str);
}

template<>
long sToNum<long>(string str){
	return stol(str);
}



#endif