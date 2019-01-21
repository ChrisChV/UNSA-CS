#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <vector>
#include <typeinfo>
#include <tuple>


using namespace std;

typedef double ValType;
typedef int IndexType;
typedef double PercentageType;
typedef vector<ValType> ValVec;
//typedef vector<boost::any> Register;
typedef vector<string> Registro;

#define NULL_VAL 0
#define INFINITO_NEGATIVO -20000

ValType stoVT(string number){
	return stof(number);
}

void printValVec(ValVec & x){
	for(ValType val : x){
		cout<<val<<"\t";
	}
	cout<<endl;
}



/*
template <typename Register>
void printRegister(Register reg){
	boost::any val;
	for(int i = 0; i < reg.size(); i++){
		val = reg[i];
		cout<<boost::any_cast<val::type_value>(&val)<<" ";
	}
	cout<<endl;
}
*/

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

/*
template <typename Register>
void printRegistro(Register reg, const int index){
	int size = tuple_size<Register>::value;
	if(index == size) return;
	cout<<get<index>(reg)<<" ";
	printRegistro<Register>(reg, index + 1);
}

template <typename Register>
void printRegistro(Register reg){
	printRegistro<Register>(reg, 0);
	cout<<endl;
}
*/

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