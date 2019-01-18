#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <tuple>
#include "Utils.h"
#include "MyTime.h"
#include "Distancias.h"

using namespace std;

typedef tuple<IndexType, ValType, PercentageType> KNNTuple;
typedef tuple<IndexType, ValVec, ValVec> ValVecTuple; // 0 indice del otro, 1 ValVec del choseOne, 2 Valvec del otro

typedef function<bool(KNNTuple,KNNTuple)> SortFun;

vector<KNNTuple> KNN(vector<ValVec> & vals, IndexType subjectIndex, int k, DistanceFunction distFun, SortFun sortFun){
	vector<KNNTuple> res;
	ValVec subject = vals[subjectIndex];
	for(IndexType i = 0; i < vals.size(); i++){
		if(subjectIndex == i) continue;
		if(vals[i].size() == 0) continue;
		res.push_back(make_tuple(i, distFun(subject, vals[i]), 0));
	}
	sort(res.begin(), res.end(), sortFun);
	res.erase(res.begin() + k, res.end());

	/*
	ValType sum = 0;
	for(KNNTuple knnT : res){
		sum += get<1>(knnT);
	}
	for(IndexType i = 0; i < res.size(); i++){
		get<2>(res[i]) = get<1>(res[i]) / sum;
	}
	*/
	return res;
}

vector<KNNTuple> KNN(vector<ValVec> & vals, IndexType subjectIndex, DistanceFunction distFun, SortFun sortFun){
	vector<KNNTuple> res;
	ValVec subject = vals[subjectIndex];
	for(IndexType i = 0; i < vals.size(); i++){
		if(subjectIndex == i) continue;
		if(vals[i].size() == 0) continue;
		res.push_back(make_tuple(i, distFun(subject, vals[i]), 0));
	}
	sort(res.begin(), res.end(), sortFun);
	return res;
}

vector<KNNTuple> KNN(vector<ValVecTuple> & vals, IndexType subjectIndex, int k, DistanceFunction distFun, SortFun sortFun){
	vector<KNNTuple> res;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(subjectIndex == get<0>(*iter)) continue;
		res.push_back(make_tuple(get<0>(*iter), distFun(get<1>(*iter),get<2>(*iter)), 0));
	}
	//[] (KNNTuple a, KNNTuple b){return get<1>(a) < get<1>(b);}
	sort(res.begin(), res.end(), sortFun);
	if(res.size() > k) res.erase(res.begin() + k, res.end());

	/*
	ValType sum = 0;
	for(KNNTuple knnT : res){
		sum += get<1>(knnT);
	}
	for(IndexType i = 0; i < res.size(); i++){
		get<2>(res[i]) = get<1>(res[i]) / sum;
	}
	*/

	return res;	
}

vector<KNNTuple> KNN(vector<ValVecTuple> & vals, IndexType subjectIndex, DistanceFunction distFun, SortFun sortFun){
	vector<KNNTuple> res;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(subjectIndex == get<0>(*iter)) continue;
		res.push_back(make_tuple(get<0>(*iter), distFun(get<1>(*iter),get<2>(*iter)), 0));
	}
	sort(res.begin(), res.end(), sortFun);
	return res;	
}

vector<KNNTuple> KNNCoseno(vector<ValVecTuple> & vals, IndexType subjectIndex, map<int,ValType> modules){
	vector<KNNTuple> res;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(subjectIndex == get<0>(*iter)) continue;
		ValType temp = cosenDistanceSinM(get<1>(*iter),get<2>(*iter));
		ValType temp2 =  modules[subjectIndex] * modules[get<0>(*iter)];
		if(temp2 == 0) res.push_back(make_tuple(get<0>(*iter), -1,0));
		else res.push_back(make_tuple(get<0>(*iter), temp / temp2,0));
	}
	sort(res.begin(), res.end(), [](KNNTuple a,KNNTuple b) {return get<1>(a) > get<1>(b);});
	return res;
}

vector<KNNTuple> KNNCoseno(vector<ValVecTuple> & vals, IndexType subjectIndex, int k ,map<int,ValType> modules){
	vector<KNNTuple> res;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(subjectIndex == get<0>(*iter)) continue;
		ValType temp = cosenDistanceSinM(get<1>(*iter),get<2>(*iter));
		ValType temp2 =  modules[subjectIndex] * modules[get<0>(*iter)];
		if(temp2 == 0) res.push_back(make_tuple(get<0>(*iter), -1,0));
		else res.push_back(make_tuple(get<0>(*iter), temp / temp2,0));
	}
	sort(res.begin(), res.end(), [](KNNTuple a,KNNTuple b) {return get<1>(a) > get<1>(b);});
	if(res.size() > k) res.erase(res.begin() + k, res.end());
	return res;
}


/*Recomienda productos(libro, película) segun el más cercano
  El ValVec valores contiene todos los valores de los productos del mas cercano*/
vector<IndexType> recomendNearest(ValVec & valores){
	ValType mayor = 0;
	vector<IndexType> actual;
	for(IndexType i = 0; i < valores.size(); i++){
		if(i == 0 or mayor < valores[i]){
			actual.clear();
			mayor = valores[i];
			actual.push_back(i);
		}
		else if(mayor == valores[i]) actual.push_back(i);
	}
	return actual;
}

/*Porcentaje Proyectado segun un solo producto
  El ValVec valores contiene las valoraciones para ese producto de los ususarios de KnnVec*/
ValType porcentajeProyectado(vector<KNNTuple> & KnnVec, ValVec valores){
	ValType res = 0;
	for(int i = 0; i < valores.size(); i++){
		if(valores[i] == NULL_VAL) continue;
		res += (ValType) valores[i] * (ValType) get<2>(KnnVec[i]);
	}
	return res;	
}

SimilarityMatrix getSimilarityMatrix(vector<ValVec> & vals){
	ValVec prom = getPromedios(vals);
	SimilarityMatrix res;
	for(int i = 0; i < vals.size(); i++){
		for(int j = i; j < vals.size(); j++){
			if(i == j) continue;
			res[i][j] = cosenoAjustado(vals[i], vals[j], prom);
		}
	}
	return res;
}

/*
SimilarityMatrixString getSimilarityMatrix(map<string, map<int,ValType>> & vals, vector<map<string, ValType>> & valsUser, int numUsers){
	SimilarityMatrixString res;
	MyTime mytime;
	cout<<"Generando promedios..."<<endl;
	mytime.init();
	auto proms = getPromedios(valsUser);
	mytime.end();
	cout<<"Done->";
	mytime.print();
	cout<<"Generando matriz de similitud..."<<endl;
	mytime.init();
	ValVec a;
	ValVec b;
	int count = 0;
	int count2 = 0;
	cout<<vals.size()<<endl;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		cout<<count++<<endl;
		count2 = 0;
		a = ValVec(numUsers, NULL_VAL);
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			a[iter2->first] = iter2->second;
		}
		for(auto iter2 = iter; iter != vals.end(); ++iter2){
			//cout<<count2++<<endl;
			if(iter == iter2) continue;
			b = ValVec(numUsers, NULL_VAL);
			for(auto iter3 = iter2->second.begin(); iter3 != iter2->second.end(); ++iter3){
				b[iter3->first] = iter3->second;
			}
			res[iter->first][iter2->first] = cosenoAjustado(a, b, proms);
		}
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();
	return res;
}*/


SimilarityVector getSimilarityVector(map<string, map<int,ValType>> & vals, vector<map<string, ValType>> & valsUser, IndexType userId, string bookId){
	SimilarityVector res;
	MyTime mytime;
	cout<<"Generando promedios..."<<endl;
	mytime.init();
	auto proms = getPromedios(valsUser);
	mytime.end();
	cout<<"Done->";
	mytime.print();
	cout<<"Generando vector de similitud..."<<endl;
	mytime.init();
	ValVec a(valsUser.size(), NULL_VAL);
	ValVec b;
	ValVec temp(valsUser.size(), NULL_VAL);
	auto actualBook = vals.find(bookId);
	if(actualBook == vals.end()){
		cout<<"El libro no tiene ranquings"<<endl;
		return res;
	}

/*
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(actualBook == iter){
			res[actualBook->first] = INFINITO_NEGATIVO;
			continue;	
		} 
		a = temp;
		b = temp;
		for(auto iter2 = iter->second.begin(); iter2 != iter2->second.end(); ++iter2){
			b[iter2->first] = iter2->second;
		}
	}
*/

	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(actualBook == iter){
			//res[actualBook->first] = INFINITO_NEGATIVO;
			continue;
		} 
		a.clear();
		b.clear();
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			auto findRes = actualBook->second.find(iter2->first);
			if(findRes == actualBook->second.end()) continue;
			a.push_back(findRes->second);
			b.push_back(iter2->second);
		}
		res[iter->first] = cosenoAjustado(a, b, proms);
	}

	/*
	for(auto iter = actualBook->second.begin(); iter != actualBook->second.end(); ++iter){
		a[iter->first] = iter->second;
	}
	cout<<a.size()<<endl;


	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		//cout<<count++<<endl;
		if(actualBook == iter){
			res[actualBook->first] = INFINITO_NEGATIVO;
			continue;	
		} 
		b = temp;
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			b[iter2->first] = iter2->second;
		}
		res[iter->first] =  cosenoAjustado(a,b, proms);
	}
	return res;
	*/
	mytime.end();
	cout<<"Done->";
	mytime.print();
	return res;
}

SimilarityMovieLensVector getSimilarityVector(map<int, map<int,ValType>> & vals, vector<map<int, ValType>> & valsUser, IndexType userId, IndexType bookId){
	SimilarityMovieLensVector res;
	MyTime mytime;
	cout<<"Generando promedios..."<<endl;
	mytime.init();
	auto proms = getPromedios(valsUser);
	mytime.end();
	cout<<"Done->";
	mytime.print();
	cout<<"Generando vector de similitud..."<<endl;
	mytime.init();
	ValVec a;
	ValVec b;
	ValVec Tprom;
	auto actualBook = vals.find(bookId);
	if(actualBook == vals.end()){
		cout<<"El libro no tiene ranquings"<<endl;
		return res;
	}

	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(actualBook == iter){
			//res[actualBook->first] = INFINITO_NEGATIVO;
			continue;	
		}
		a.clear();
		b.clear();
		Tprom.clear();
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			auto findRes = actualBook->second.find(iter2->first);
			if(findRes == actualBook->second.end()) continue;
			a.push_back(findRes->second);
			b.push_back(iter2->second);
			Tprom.push_back(proms[iter2->first]);
		}
		if(a.size() == 0) continue;
		if(iter->first == 31){
			printValVec(Tprom);
			cout<<endl;
		}
		res[iter->first] = cosenoAjustado(a, b, Tprom);
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();
	return res;
}

MovieLensVectorDesviacion getvectorDesviacion(map<int, map<int,ValType>> & vals, vector<map<int, ValType>> & valsUser, IndexType userId, IndexType movieId){
	MovieLensVectorDesviacion res;
	ValVec a;
	ValVec b;
	auto actualMovie = vals.find(movieId);
	map<int,ValType> * actualUser = &valsUser[userId];
	if(actualMovie == vals.end()){
		cout<<"El libro no tiene ranqkings"<<endl;
		return res;
	}
	map<int,ValType>::iterator findMovie;
	map<int,ValType>::iterator findUser;
	for(auto iter = vals.begin(); iter != vals.end(); ++iter){
		if(actualMovie == iter) continue;
		findMovie = actualUser->find(iter->first);
		if(findMovie == actualUser->end()) continue;
		a.clear();
		b.clear();
		for(auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2){
			findUser = actualMovie->second.find(iter2->first);
			if(findUser == actualMovie->second.end()) continue;
			a.push_back(findUser->second);
			b.push_back(iter2->second);
		}
		//if(a.size() == 0 or a.size() == 1) continue;
		if(a.size() == 0) continue;
		cout<<iter->first<<endl;
		res[iter->first] = make_tuple(desviacionEstandar(a,b), a.size());
	}
	return res;
}


ValType prediccionPorProducto(SimilarityMatrix & matrix, vector<ValVec> & vals, IndexType productIndex, IndexType userIndex){
	ValType d = 0;
	ValType t = 0;
	for(int i = 0; i < vals.size(); i++){
		if(i == productIndex) continue;
		if(vals[i][userIndex] == INFINITO_NEGATIVO) continue;
		auto temp = searchInSimilarityMatrix(matrix, productIndex, i);
		d += temp * vals[i][userIndex];
		t += abs(temp);
	}
	cout<<"D->"<<d<<endl;
	cout<<"T->"<<t<<endl;
	return (ValType) d / t;
}

ValType prediccionPorProducto(SimilarityVector & vec, vector<map<string, ValType>> & valsUser, IndexType userId){
	ValType d = 0;
	ValType t = 0;
	for(auto iter = vec.begin(); iter != vec.end(); ++iter){
		auto findRes = valsUser[userId].find(iter->first);
		if(findRes == valsUser[userId].end()) continue;
		//if(iter->second == -1){
			//cout<<iter->second<<endl;
			//cout<<t<<endl;
		//	continue;
		//}
		cout<<valsUser[userId][iter->first]<<" "<<iter->second<<endl;

		d += iter->second * valsUser[userId][iter->first];
		t += abs(iter->second);
	}
	return (ValType) d / t;
}

ValType prediccionPorProducto(SimilarityMovieLensVector & vec, vector<map<int, ValType>> & valsUser, IndexType userId){
	ValType d = 0;
	ValType t = 0;
	for(auto iter = vec.begin(); iter != vec.end(); ++iter){
		auto findRes = valsUser[userId].find(iter->first);
		if(findRes == valsUser[userId].end()) continue;
		cout<<"I->"<<iter->first<<endl;
		cout<<"CA->"<<iter->second<<endl;
		d += iter->second * valsUser[userId][iter->first];
		t += abs(iter->second);
	}
	if(t == 0) return INFINITO_NEGATIVO;
	if(isnan(d) or isnan(t)){
		cout<<"Valor Nan"<<endl;
		return INFINITO_NEGATIVO;
	} 
	return (ValType) d / t;
}
	
ValType slopeOne(MovieLensVectorDesviacion & vec, map<int, map<int,ValType>> & vals, vector<map<int, ValType>> & valsUser, IndexType userId, IndexType movieId){
	ValType n = 0;
	ValType d = 0;
	map<int,ValType> * actualUser = &valsUser[userId];
	MovieLensVectorDesviacion::iterator findRes;
	for(auto iter = actualUser->begin(); iter != actualUser->end(); ++iter){
		findRes = vec.find(iter->first);
		n += (get<0>(findRes->second) + iter->second) * get<1>(findRes->second);
		d += get<1>(findRes->second);
	}
	if(d == 0) return 0;
	return (ValType) n / d;
}



#endif