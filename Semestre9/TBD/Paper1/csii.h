#ifndef CSII_H
#define CSII_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <thread>
#include "connector.h"

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <random>
#include <ctime>
#include <algorithm>


using namespace std;
using namespace boost::numeric::ublas;

#define NUM_THREADS 4

enum VALS{CERO_VAL, ST_VAL, S_VAL, T_VAL};

typedef double ValType;
typedef int UserId;
typedef int ItemId;
typedef int IndexType;
typedef int Cardinalidad;

const string BDName = "ml";

bool invertMatrix(mapped_matrix<ValType> & input, mapped_matrix<ValType> & inversa){
	mapped_matrix<ValType> A(input);
	permutation_matrix<size_t> pm(A.size1());
	int res = lu_factorize(A,pm);
	if(res != 0) return false;

	inversa.assign(identity_matrix<ValType>(A.size1()));

	lu_substitute(A,pm,inversa);

	return true;
}


void getPQU(matrix<ValType> & P, matrix<ValType> & Q, std::vector<ValType> & PQU , int userIndex){
	PQU.clear();
	PQU.shrink_to_fit();
	PQU = std::vector<ValType>(Q.size2(), 0);
	for(int i = 0; i < Q.size2(); i++){
		for(int j = 0; j < P.size2(); j++){
			PQU[i] += P(userIndex,j) * Q(j,i);
		}
	}
}



///CUIDADO CON MANTENER LA CONEXION ABIERTA MUCHO TIEMPO

void getN_ofItems(std::vector<Cardinalidad> & res, int numItems, MYSQL * conn){
	res.clear();
	res.shrink_to_fit();
	res = std::vector<Cardinalidad>(numItems, 0);
	cout<<"InitQuery..."<<endl;
	MYSQL_RES * resQuery = getQueryRes(conn, "select MovieId, COUNT(UserId) from ratings group by MovieId");
	cout<<"EndQuery"<<endl;
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	while((row = mysql_fetch_row(resQuery))){
		res[atoi(row[0])] = atoi(row[1]);
	}
	mysql_free_result(resQuery);
}

void getNumIR_ofUsers(std::vector<Cardinalidad> & res, int numUsers, MYSQL * conn){
	res.clear();
	res.shrink_to_fit();
	res = std::vector<Cardinalidad>(numUsers, 0);
	MYSQL_RES * resQuery = getQueryRes(conn, "select UserId, COUNT(MovieId) from ratings group by UserId");
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	while((row = mysql_fetch_row(resQuery))){
		res[atoi(row[0])] = atoi(row[1]);
	}
	mysql_free_result(resQuery);
}

void getUmbrales(std::vector<ValType> & res, int numItems, MYSQL * conn){
	res.clear();
	res.shrink_to_fit();
	res = std::vector<ValType>(numItems, 0);
	MYSQL_RES * resQuery = getQueryRes(conn, "select MovieID, SUM(Puntaje) / COUNT(MovieId) from ratings group by MovieID");
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	while((row = mysql_fetch_row(resQuery))){
		res[atoi(row[0])] = atof(row[1]);
	}
	mysql_free_result(resQuery);		
}


void getT_user(mapped_vector<ValType> & res, UserId id, int vectorSize, std::vector<IndexType> & indexVec, MYSQL * conn){
	mapped_vector<ValType>(vectorSize).swap(res);
	indexVec.clear();
	indexVec.shrink_to_fit();
	MYSQL_RES * resQuery = getQueryRes(conn, "select MovieId from ratings where userId = " + to_string(id));
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	IndexType index = 0;
	while((row = mysql_fetch_row(resQuery))){
		index = atoi(row[0]);
		indexVec.push_back(index);
		res[index] = 1;
	}
	mysql_free_result(resQuery);
}

void getT_item(mapped_vector<ValType> & res, ItemId id, int vectorSize, std::vector<IndexType> & indexVec, MYSQL * conn){
	mapped_vector<ValType>(vectorSize).swap(res);
	indexVec.clear();
	indexVec.shrink_to_fit();
	MYSQL_RES * resQuery = getQueryRes(conn, "select UserId from ratings where MovieId = " + to_string(id));
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	IndexType index = 0;
	while((row = mysql_fetch_row(resQuery))){
		index = atoi(row[0]);
		indexVec.push_back(index);
		res[index] = 1;
	}
	mysql_free_result(resQuery);
}

//Vamos a ponder los indices 1 en indexVec, los 0 no
void getS_user(mapped_vector<ValType> & res, UserId id, int vectorSize, 
				std::vector<IndexType> & indexVec, std::vector<IndexType> & indexCeroVec,
					 std::vector<ValType> umbrales, MYSQL * conn){
	mapped_vector<ValType>(vectorSize).swap(res);
	indexVec.clear();
	indexVec.shrink_to_fit();
	indexCeroVec.clear();
	indexCeroVec.shrink_to_fit();
	MYSQL_RES * resQuery = getQueryRes(conn, "select MovieId, Puntaje from ratings where userId = " + to_string(id));
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	IndexType index = 0;
	while((row = mysql_fetch_row(resQuery))){
		index = atoi(row[0]);
		if(atof(row[1]) >= umbrales[index]){
			res[index] = 1;
			indexVec.push_back(index);	
		} 
		else{
			res[index] = 0;
			indexCeroVec.push_back(index);
		} 
	}
	mysql_free_result(resQuery);
}

void getS_item(mapped_vector<ValType> & res, ItemId id, int vectorSize, 
				std::vector<IndexType> & indexVec, std::vector<IndexType> & indexCeroVec,
					 std::vector<ValType> umbrales, MYSQL * conn){
	mapped_vector<ValType>(vectorSize).swap(res);
	indexVec.clear();
	indexVec.shrink_to_fit();
	indexCeroVec.clear();
	indexCeroVec.shrink_to_fit();
	MYSQL_RES * resQuery = getQueryRes(conn, "select UserId, Puntaje from ratings where movieId = " + to_string(id));
	if(resQuery == NULL) return;
	MYSQL_ROW row;
	IndexType index = 0;
	ValType umbral = umbrales[id];
	while((row = mysql_fetch_row(resQuery))){
		index = atoi(row[0]);
		if(atof(row[1]) >= umbral){
			res[index] = 1;
			indexVec.push_back(index);	
		} 
		else{
			res[index] = 0;
			indexCeroVec.push_back(index);
		} 
	}
	mysql_free_result(resQuery);
}


void getInitPorQ(matrix<ValType> & res, int size1, int k){
	matrix<ValType>(size1, k).swap(res);
	default_random_engine generator;
  	normal_distribution<ValType> distribution(0,0.01);
  	generator.seed(time(NULL));
  	for(int i = 0; i < size1; i++){
  		for(int j = 0; j < k; j++){
  			res(i,j) =  distribution(generator);
  		}
  	}
}

void getUserWT(mapped_matrix<ValType> & res, ValType & sumRes, UserId id, mapped_vector<ValType> & T, std::vector<IndexType> & indexVec,
				int numUsers, int numItems, std::vector<Cardinalidad> & N_ofItems){
	mapped_matrix<ValType>(numItems, numItems).swap(res);
	sumRes = 0;
	Cardinalidad numIR = indexVec.size();
	Cardinalidad numInoR = numItems - numIR;
	ValType left = (ValType) numInoR / (ValType) (numIR + numInoR);
	ValType valRes = 0;
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		valRes = left + (ValType) N_ofItems[(*iter)] / (ValType) numUsers;
		res((*iter),(*iter)) =  valRes;
		sumRes += valRes;
	}
}

void getItemWT(mapped_matrix<ValType> & res, ValType & sumRes ,ItemId id, mapped_vector<ValType> & T, std::vector<IndexType> & indexVec,
				int numUsers, int numItems, std::vector<Cardinalidad> & numIR_ofUsers){
	mapped_matrix<ValType>(numUsers,numUsers).swap(res);
	sumRes = 0;
	ValType right = (ValType) indexVec.size() / (ValType) numUsers;
	Cardinalidad numIR = 0;
	Cardinalidad numInoR = 0;
	ValType valRes = 0;
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		numIR = numIR_ofUsers[(*iter)];
		numInoR = numItems - numIR;
		valRes = ((ValType) numInoR / (ValType) (numIR + numInoR)) + right;
		res((*iter),(*iter)) = valRes;
		sumRes += valRes;
	}
}


void getUserWS(mapped_matrix<ValType> & res, ValType & sumRes, UserId id, mapped_vector<ValType> & S,
				std::vector<IndexType> & indexVec, std::vector<IndexType> & indexCeroVec,
					int numUsers, int numItems, std::vector<Cardinalidad> & N_ofItems){
	mapped_matrix<ValType>(numItems, numItems).swap(res);
	sumRes = 0;
	Cardinalidad numIR = indexVec.size() + indexCeroVec.size();
	Cardinalidad numInoR = numItems - numIR;
	Cardinalidad numISR = indexVec.size();
	ValType left = (ValType) numInoR / (ValType) (numIR + numInoR);
	ValType SUI1 = (ValType) (numIR - numISR) / (ValType) numIR;
	ValType SUI0 = (ValType) numISR / (ValType) numIR;
	ValType valRes = 0;
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		valRes = left + SUI1 + (ValType) N_ofItems[(*iter)] / (ValType) numUsers;
		res((*iter),(*iter)) = valRes;
		sumRes += valRes;
	}
	for(auto iter = indexCeroVec.begin(); iter != indexCeroVec.end(); ++iter){
		valRes = left + SUI0 + (ValType) N_ofItems[(*iter)] / (ValType) numUsers;
		res((*iter),(*iter)) = valRes;
		sumRes += valRes;
	}
}

void getItemWS(mapped_matrix<ValType> & res, ValType & sumRes, UserId id, mapped_vector<ValType> & S,
				std::vector<IndexType> & indexVec, std::vector<IndexType> & indexCeroVec,
					int numUsers, int numItems, std::vector<Cardinalidad> & numIR_ofUsers,
					 std::vector<Cardinalidad> & numISR_ofUsers){
	mapped_matrix<ValType>(numUsers,numUsers).swap(res);
	sumRes = 0;
	ValType right = (ValType) (indexVec.size() + indexCeroVec.size()) / (ValType) numUsers;
	ValType valRes = 0;
	Cardinalidad numIR = 0;
	Cardinalidad numInoR = 0;
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		numIR = numIR_ofUsers[(*iter)];
		numInoR = numItems - numIR;	
		valRes = ((ValType) numInoR / (ValType) (numIR + numInoR)) + ( (ValType) (numIR - numISR_ofUsers[(*iter)]) / (ValType) numIR) + right;
		res((*iter),(*iter)) = valRes;
		sumRes += valRes;
	}
	for(auto iter = indexCeroVec.begin(); iter != indexCeroVec.end(); ++ iter){
		numIR = numIR_ofUsers[(*iter)];
		numInoR = numItems - numIR;	
		valRes = ((ValType) numInoR / (ValType) (numIR + numInoR)) + ( (ValType) numISR_ofUsers[(*iter)] / (ValType) numIR) + right;
		res((*iter),(*iter)) = valRes;
		sumRes += valRes;	
	}
}

void vectorXmatrixIdentidad(mapped_vector<ValType> & vec, mapped_matrix<ValType> & matx, 
						std::vector<IndexType> & indexVec, mapped_vector<ValType> & res){
	mapped_vector<ValType>(vec.size()).swap(res);
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		res[(*iter)] = vec[(*iter)] * matx((*iter),(*iter));
	}
}

void vectorXmatrix(mapped_vector<ValType> & vec, matrix<ValType> & matx,
					std::vector<IndexType> & indexVec, mapped_vector<ValType> & res){
	mapped_vector<ValType>(matx.size2()).swap(res);
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		for(int i = 0; i < matx.size2(); i++){
			res[i] += vec[(*iter)] * matx((*iter), i);
		}
	}
}

void matrixXmatrixIdentidad(matrix<ValType> & matrixA, mapped_matrix<ValType> & matrixB,
							std::vector<IndexType> & indexVec, mapped_matrix<ValType> & res){
	mapped_matrix<ValType>(matrixA.size1(),matrixB.size2()).swap(res);
	ValType tempVal = 0;
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		tempVal = matrixB((*iter),(*iter));
		for(int i = 0; i < matrixA.size1(); i++){
			res(i,(*iter)) = matrixA(i,(*iter)) * tempVal;
		}
	}
}

void matrixXmatrix(mapped_matrix<ValType> & matrixA, matrix<ValType> & matrixB,
						std::vector<IndexType> & indexVec, mapped_matrix<ValType> & res){
	mapped_matrix<ValType>(matrixA.size1(),matrixB.size2()).swap(res);
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		for(int i = 0; i < matrixA.size1(); i++){
			for(int j = 0; j < matrixB.size2(); j++){
				res(i,j) += matrixA(i,((*iter))) * matrixB((*iter),j);
			}
		}
	}
}

void parallelMatrixXMatrix(matrix<ValType> * matrixA, matrix<ValType> * matrixB, matrix<ValType> * res,
							int ini, int end){
	for(int i = ini; i < end; i++){
		for(int j = 0; j < matrixB->size2(); j++){
			for(int k = 0; k < matrixA->size2(); k++){
				(*res)(i,j) += (*matrixA)(i,k) * (*matrixB)(j,k);
			}
		}
	}
}

void matrixXmatrix(matrix<ValType> & matrixA, matrix<ValType> & matrixB, matrix<ValType> & res){
	res = matrix<ValType>(matrixA.size1(), matrixB.size2());
	thread threads[NUM_THREADS];
	int h = matrixA.size1() / NUM_THREADS;
	int ini = 0;
	int end = 0;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = matrixA.size1();
		else end = ini + h;
		threads[i] = thread(parallelMatrixXMatrix, &matrixA, &matrixB, &res, ini, end);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}
}


void parallelP_S(matrix<ValType> * P, matrix<ValType> * Q_trans, matrix<ValType> * Q,
				 std::vector<Cardinalidad> * N_ofItems, std::vector<ValType> * umbrales, 
				 std::vector<Cardinalidad> * numISR_ofUsers, ValType regPar, int ini, int end, 
				 	int Psize1, int Psize2){
	mapped_vector<ValType> S;
	std::vector<IndexType> indexVec;
	std::vector<IndexType> indexCeroVec;
	mapped_matrix<ValType> WS;
	identity_matrix<ValType> I(Psize2);
	mapped_matrix<ValType> rsI;
	ValType sumW = 0;

	mapped_vector<ValType> S_WS;
	mapped_vector<ValType> S_WS_Q;

	mapped_matrix<ValType> Q_trans_WS;
	mapped_matrix<ValType> Q_trans_WS_Q;
	mapped_matrix<ValType> Q_trans_WS_Q_inversa;

	mapped_vector<ValType> resVec;

	ValType regPar_sumW = 0;

	MYSQL * conn = getConnect(BDName);

	for(int i = ini; i < end; i++){
		mapped_matrix<ValType>(Psize2,Psize2).swap(rsI);
		mapped_matrix<ValType>(Psize2,Psize2).swap(Q_trans_WS_Q_inversa);
		mapped_vector<ValType>(Psize2).swap(resVec);
		rsI = mapped_matrix<ValType>(I);
		getS_user(S, i, Q->size1(), indexVec, indexCeroVec, *umbrales, conn);
		if(indexVec.size() == 0 and indexCeroVec.size() == 0) continue;	
		(*numISR_ofUsers)[i] = indexVec.size();
		getUserWS(WS, sumW, i, S, indexVec, indexCeroVec, Psize1, Q->size1(), *N_ofItems);
		vectorXmatrixIdentidad(S, WS, indexVec, S_WS);
		vectorXmatrix(S_WS, *Q, indexVec ,S_WS_Q);

		matrixXmatrixIdentidad(*Q_trans, WS, indexVec, Q_trans_WS);
		matrixXmatrix(Q_trans_WS, *Q, indexVec, Q_trans_WS_Q);


		regPar_sumW = regPar * sumW;
		rsI *= regPar_sumW;

		Q_trans_WS_Q += rsI;

		if(invertMatrix(Q_trans_WS_Q, Q_trans_WS_Q_inversa) == false){
			cout<<"--------WARNING-------"<<endl;
			cout<<"No se puede calcular inversa"<<endl;
			int tt;
			cin>>tt;
		}

		axpy_prod(S_WS_Q, Q_trans_WS_Q_inversa, resVec, true);


		row(*P, i) = resVec;
	}

	mapped_vector<ValType>().swap(S);
	indexVec.clear();
	indexVec.shrink_to_fit();
	indexCeroVec.clear();
	indexCeroVec.shrink_to_fit();
	mapped_matrix<ValType>().swap(WS);
	identity_matrix<ValType>().swap(I);
	mapped_matrix<ValType>().swap(rsI);

	mapped_vector<ValType>().swap(S_WS);
	mapped_vector<ValType>().swap(S_WS_Q);

	mapped_matrix<ValType>().swap(Q_trans_WS);
	mapped_matrix<ValType>().swap(Q_trans_WS_Q);
	mapped_matrix<ValType>().swap(Q_trans_WS_Q_inversa);

	mapped_vector<ValType>().swap(resVec);

	closeConnect(conn);
}

void operateP_S(matrix<ValType> & P, matrix<ValType> & Q, std::vector<Cardinalidad> & N_ofItems, 
					std::vector<ValType> & umbrales, std::vector<Cardinalidad> & numISR_ofUsers, 
						ValType regPar){
	thread threads[NUM_THREADS];
	matrix<ValType>  Q_trans = trans(Q);
	int Psize1 = P.size1();
	int Psize2 = P.size2();
	int h = (Psize1 - 1) / NUM_THREADS;
	int ini = 1;
	int end = ini;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = Psize1;
		else end = ini + h;
		threads[i] = thread(parallelP_S, &P, &Q_trans, &Q, &N_ofItems, &umbrales, &numISR_ofUsers, regPar, ini, end, Psize1, Psize2);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}

	matrix<ValType>().swap(Q_trans);
}

void parallelP_T(matrix<ValType> * P, matrix<ValType> * Q_trans, matrix<ValType> * Q, 
				std::vector<Cardinalidad> * N_ofItems, ValType regPar,
				int ini, int end, int Psize1, int Psize2){
	mapped_vector<ValType> T;
	std::vector<IndexType> indexVec;
	mapped_matrix<ValType> WT;
	identity_matrix<ValType> I(Psize2);
	mapped_matrix<ValType> rsI;
	ValType sumW = 0;

	mapped_vector<ValType> T_WT;
	mapped_vector<ValType> T_WT_Q;

	mapped_matrix<ValType> Q_trans_WT;
	mapped_matrix<ValType> Q_trans_WT_Q;
	mapped_matrix<ValType> Q_trans_WT_Q_inversa;

	mapped_vector<ValType> resVec;

	ValType regPar_sumW = 0;

	MYSQL * conn = getConnect(BDName);

	for(int i = ini; i < end; i++){
		mapped_matrix<ValType>(Psize2,Psize2).swap(rsI);
		mapped_matrix<ValType>(Psize2,Psize2).swap(Q_trans_WT_Q_inversa);
		mapped_vector<ValType>(Psize2).swap(resVec);
		rsI = mapped_matrix<ValType>(I);

		getT_user(T, i, Q->size1(), indexVec, conn);
		if(indexVec.size() == 0) continue;
		getUserWT(WT, sumW, i, T, indexVec, Psize1, Q->size1(), *N_ofItems);
		vectorXmatrixIdentidad(T, WT, indexVec, T_WT);
		vectorXmatrix(T_WT, *Q, indexVec ,T_WT_Q);

		matrixXmatrixIdentidad(*Q_trans, WT, indexVec, Q_trans_WT);
		matrixXmatrix(Q_trans_WT, *Q, indexVec, Q_trans_WT_Q);


		regPar_sumW = regPar * sumW;
		rsI *= regPar_sumW;

		Q_trans_WT_Q += rsI;

		if(invertMatrix(Q_trans_WT_Q, Q_trans_WT_Q_inversa) == false){
			cout<<"--------WARNING-------"<<endl;
			cout<<"No se puede calcular inversa"<<endl;
			int tt;
			cin>>tt;
		}

		axpy_prod(T_WT_Q, Q_trans_WT_Q_inversa, resVec, true);


		row(*P, i) = resVec;
	}

	mapped_vector<ValType>().swap(T);
	indexVec.clear();
	indexVec.shrink_to_fit();
	mapped_matrix<ValType>().swap(WT);
	identity_matrix<ValType>().swap(I);
	mapped_matrix<ValType>().swap(rsI);

	mapped_vector<ValType>().swap(T_WT);
	mapped_vector<ValType>().swap(T_WT_Q);

	mapped_matrix<ValType>().swap(Q_trans_WT);
	mapped_matrix<ValType>().swap(Q_trans_WT_Q);
	mapped_matrix<ValType>().swap(Q_trans_WT_Q_inversa);

	mapped_vector<ValType>().swap(resVec);

	closeConnect(conn);
}


void operateP_T(matrix<ValType> & P, matrix<ValType> & Q, std::vector<Cardinalidad> & N_ofItems, 
					ValType regPar){
	thread threads[NUM_THREADS];
	matrix<ValType>  Q_trans = trans(Q);
	int Psize1 = P.size1();
	int Psize2 = P.size2();
	int h = (Psize1 - 1) / NUM_THREADS;
	int ini = 1;
	int end = ini;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = Psize1;
		else end = ini + h;
		threads[i] = thread(parallelP_T, &P, &Q_trans, &Q, &N_ofItems, regPar, ini, end, Psize1, Psize2);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}

	matrix<ValType>().swap(Q_trans);
}


void parallelQ_S(matrix<ValType> * P, matrix<ValType> * P_trans, matrix<ValType> * Q,
				 std::vector<Cardinalidad> * numIR_ofUsers, std::vector<ValType> * umbrales, 
				 std::vector<Cardinalidad> * numISR_ofUsers, ValType regPar, int ini, int end, 
				 	int Qsize1, int Qsize2){
	mapped_vector<ValType> S;
	std::vector<IndexType> indexVec;
	std::vector<IndexType> indexCeroVec;
	mapped_matrix<ValType> WS;
	identity_matrix<ValType> I(Qsize2);
	mapped_matrix<ValType> rsI;
	ValType sumW = 0;

	mapped_vector<ValType> S_WS;
	mapped_vector<ValType> S_WS_P;

	mapped_matrix<ValType> P_trans_WS;
	mapped_matrix<ValType> P_trans_WS_P;
	mapped_matrix<ValType> P_trans_WS_P_inversa;

	mapped_vector<ValType> resVec;

	ValType regPar_sumW = 0;

	MYSQL * conn = getConnect(BDName);

	for(int i = ini; i < end; i++){
		mapped_matrix<ValType>(Qsize2,Qsize2).swap(rsI);
		mapped_matrix<ValType>(Qsize2,Qsize2).swap(P_trans_WS_P_inversa);
		mapped_vector<ValType>(Qsize2).swap(resVec);
		rsI = mapped_matrix<ValType>(I);
		getS_item(S, i, P->size1(), indexVec, indexCeroVec, *umbrales, conn);
		if(indexVec.size() == 0 and indexCeroVec.size() == 0) continue;	
		getItemWS(WS, sumW, i, S, indexVec, indexCeroVec, P->size1(), Qsize1, *numIR_ofUsers, *numISR_ofUsers);
		vectorXmatrixIdentidad(S, WS, indexVec, S_WS);
		vectorXmatrix(S_WS, *P, indexVec, S_WS_P);


		matrixXmatrixIdentidad(*P_trans, WS, indexVec, P_trans_WS);
		matrixXmatrix(P_trans_WS, *P, indexVec, P_trans_WS_P);


		regPar_sumW = regPar * sumW;
		rsI *= regPar_sumW;

		P_trans_WS_P += rsI;

		if(invertMatrix(P_trans_WS_P, P_trans_WS_P_inversa) == false){
			cout<<"--------WARNING-------"<<endl;
			cout<<"No se puede calcular inversa"<<endl;
			int tt;
			cin>>tt;
		}

		axpy_prod(S_WS_P, P_trans_WS_P_inversa, resVec, true);

		row(*Q, i) = resVec;
	}

	closeConnect(conn);
	mapped_vector<ValType>().swap(S);
	indexVec.clear();
	indexVec.shrink_to_fit();
	indexCeroVec.clear();
	indexCeroVec.shrink_to_fit();
	mapped_matrix<ValType>().swap(WS);
	identity_matrix<ValType>().swap(I);
	mapped_matrix<ValType>().swap(rsI);

	mapped_vector<ValType>().swap(S_WS);
	mapped_vector<ValType>().swap(S_WS_P);

	mapped_matrix<ValType>().swap(P_trans_WS);
	mapped_matrix<ValType>().swap(P_trans_WS_P);
	mapped_matrix<ValType>().swap(P_trans_WS_P_inversa);

	mapped_vector<ValType>().swap(resVec);
}

void operateQ_S(matrix<ValType> & P, matrix<ValType> & Q, std::vector<Cardinalidad> & numIR_ofUsers, 
				std::vector<ValType> & umbrales, std::vector<Cardinalidad> & numISR_ofUsers,
				ValType regPar){
	thread threads[NUM_THREADS];
	matrix<ValType>  P_trans = trans(P);
	int Qsize1 = Q.size1();
	int Qsize2 = Q.size2();
	int h = (Qsize1 - 1) / NUM_THREADS;
	int ini = 1;
	int end = ini;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = Qsize1;
		else end = ini + h;
		threads[i] = thread(parallelQ_S, &P, &P_trans, &Q, &numIR_ofUsers, &umbrales, &numISR_ofUsers, regPar, ini, end, Qsize1, Qsize2);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}

	matrix<ValType>().swap(P_trans);

}

void parallelQ_T(matrix<ValType> * P, matrix<ValType> * P_trans, matrix<ValType> * Q, 
				std::vector<Cardinalidad> * numIR_ofUsers, ValType regPar,
				int ini, int end, int Qsize1, int Qsize2){
	mapped_vector<ValType> T;
	std::vector<IndexType> indexVec;
	mapped_matrix<ValType> WT;
	identity_matrix<ValType> I(Qsize2);
	mapped_matrix<ValType> rsI;
	ValType sumW = 0;

	mapped_vector<ValType> T_WT;
	mapped_vector<ValType> T_WT_P;

	mapped_matrix<ValType> P_trans_WT;
	mapped_matrix<ValType> P_trans_WT_P;
	mapped_matrix<ValType> P_trans_WT_P_inversa;

	mapped_vector<ValType> resVec;

	ValType regPar_sumW = 0;

	MYSQL * conn = getConnect(BDName);

	for(int i = ini; i < end; i++){
		mapped_matrix<ValType>(Qsize2,Qsize2).swap(rsI);
		mapped_matrix<ValType>(Qsize2,Qsize2).swap(P_trans_WT_P_inversa);
		mapped_vector<ValType>(Qsize2).swap(resVec);
		rsI = mapped_matrix<ValType>(I);

		getT_item(T, i, P->size1(), indexVec, conn);
		if(indexVec.size() == 0) continue;
		getItemWT(WT, sumW, i, T, indexVec, P->size1(), Qsize1, *numIR_ofUsers);
		vectorXmatrixIdentidad(T, WT, indexVec, T_WT);
		vectorXmatrix(T_WT, *P, indexVec, T_WT_P);


		matrixXmatrixIdentidad(*P_trans, WT, indexVec, P_trans_WT);
		matrixXmatrix(P_trans_WT, *P, indexVec, P_trans_WT_P);


		regPar_sumW = regPar * sumW;
		rsI *= regPar_sumW;

		P_trans_WT_P += rsI;

		if(invertMatrix(P_trans_WT_P, P_trans_WT_P_inversa) == false){
			cout<<"--------WARNING-------"<<endl;
			cout<<"No se puede calcular inversa"<<endl;
			int tt;
			cin>>tt;
		}

		axpy_prod(T_WT_P, P_trans_WT_P_inversa, resVec, true);

		row(*Q, i) = resVec;
	}

	closeConnect(conn);
	mapped_vector<ValType>().swap(T);
	indexVec.clear();
	indexVec.shrink_to_fit();
	mapped_matrix<ValType>().swap(WT);
	identity_matrix<ValType>().swap(I);
	mapped_matrix<ValType>().swap(rsI);

	mapped_vector<ValType>().swap(T_WT);
	mapped_vector<ValType>().swap(T_WT_P);

	mapped_matrix<ValType>().swap(P_trans_WT);
	mapped_matrix<ValType>().swap(P_trans_WT_P);
	mapped_matrix<ValType>().swap(P_trans_WT_P_inversa);

	mapped_vector<ValType>().swap(resVec);
}


void operateQ_T(matrix<ValType> & P, matrix<ValType> & Q, std::vector<Cardinalidad> & numIR_ofUsers, 
					ValType regPar){
	thread threads[NUM_THREADS];
	matrix<ValType>  P_trans = trans(P);
	int Qsize1 = Q.size1();
	int Qsize2 = Q.size2();
	int h = (Qsize1 - 1) / NUM_THREADS;
	int ini = 1;
	int end = ini;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = Qsize1;
		else end = ini + h;
		threads[i] = thread(parallelQ_T, &P, &P_trans, &Q, &numIR_ofUsers, regPar, ini, end, Qsize1, Qsize2);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}

	matrix<ValType>().swap(P_trans);

}





/// ATENCION ATENCION ATENCIONA ATENCION
/// 		 ATENCION ATENCION
/// ATENCION ATENCION ATENCIONA ATENCION
/// el numUser debe ser el realNumUser + 1, igual con el numItems; esto porque en la base de datos no existe
/// el usuario 0, pero en las matricees y en los vectores si se va a tomar esta columna y fila.

/// Esta funcion calcula el PQ de T y el PQ de S. Calcula toda la matriz.
void rWPMF(matrix<ValType> & PQ_T,  matrix<ValType> & PQ_S, int numUsers, int numItems ,int k,
				 ValType regPar, int numIter , MYSQL * conn){
	matrix<ValType> P;
	matrix<ValType> Q; 
	std::vector<Cardinalidad> N_ofItems;
	std::vector<Cardinalidad> numIR_ofUsers;
	std::vector<Cardinalidad> numISR_ofUsers(numUsers, 0);
	std::vector<ValType> umbrales;

	cout<<"InitP"<<endl;
	getInitPorQ(P, numUsers, k);
	cout<<"InitQ"<<endl;
	getInitPorQ(Q, numItems, k);
	cout<<"Get N of Items"<<endl;
	getN_ofItems(N_ofItems, numItems, conn);
	cout<<"Get Num IR of Users"<<endl;
	getNumIR_ofUsers(numIR_ofUsers, numUsers, conn);
	
	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		cout<<"Calculando P_T"<<endl;
		operateP_T(P, Q, N_ofItems, regPar);
		cout<<"Calculando Q_T"<<endl;
		operateQ_T(P, Q, numIR_ofUsers, regPar);
	}
	cout<<"Calculando PQ_T"<<endl<<endl;
	matrix<ValType> Q_trans = trans(Q);
	matrix<ValType>(P.size1(),Q_trans.size2()).swap(PQ_T);
	axpy_prod(P, Q_trans, PQ_T, true);

	getInitPorQ(P, numUsers, k);
	getInitPorQ(Q, numItems, k);
	getUmbrales(umbrales, numItems, conn);

	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		cout<<"Calculando P_S"<<endl;
		operateP_S(P, Q, N_ofItems, umbrales, numISR_ofUsers, regPar);
		cout<<"Calculando Q_S"<<endl;
		operateQ_S(P, Q, numIR_ofUsers, umbrales, numISR_ofUsers, regPar);
	}

	cout<<"Calculando PQ_S"<<endl<<endl;
	matrix<ValType>().swap(Q_trans);
	Q_trans = trans(Q);
	matrix<ValType>(P.size1(),Q_trans.size2()).swap(PQ_S);


	axpy_prod(P, Q_trans, PQ_S, true);

//	matrixXmatrix(P, Q_trans, PQ_S);

	matrix<ValType>().swap(P);
	matrix<ValType>().swap(Q);
	N_ofItems.clear();
	N_ofItems.shrink_to_fit();
	numIR_ofUsers.clear();
	numIR_ofUsers.shrink_to_fit();
	numISR_ofUsers.clear();
	numISR_ofUsers.shrink_to_fit();
	umbrales.clear();
	umbrales.shrink_to_fit();
	matrix<ValType>().swap(Q_trans);
}

//Funcion parecida a la primera pero solo calcula los P y Q de T y S. No calcula la matriz entera.
void rWPMF(matrix<ValType> & P_T,  matrix<ValType> & Q_T,  matrix<ValType> & P_S, matrix<ValType> & Q_S, 
				int numUsers, int numItems ,int k, ValType regPar, int numIter , MYSQL * conn){
	std::vector<Cardinalidad> N_ofItems;
	std::vector<Cardinalidad> numIR_ofUsers;
	std::vector<Cardinalidad> numISR_ofUsers(numUsers, 0);
	std::vector<ValType> umbrales;

	cout<<"InitP"<<endl;
	getInitPorQ(P_T, numUsers, k);
	cout<<"InitQ"<<endl;
	getInitPorQ(Q_T, numItems, k);
	cout<<"Get N of Items"<<endl;
	getN_ofItems(N_ofItems, numItems, conn);
	cout<<"Get Num IR of Users"<<endl;
	getNumIR_ofUsers(numIR_ofUsers, numUsers, conn);
	
	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		cout<<"Calculando P_T"<<endl;
		operateP_T(P_T, Q_T, N_ofItems, regPar);
		cout<<"Calculando Q_T"<<endl;
		operateQ_T(P_T, Q_T, numIR_ofUsers, regPar);
	}

	cout<<"Calculanto Transpuesta de Q_T"<<endl<<endl;
	Q_T = trans(Q_T);

	getInitPorQ(P_S, numUsers, k);
	getInitPorQ(Q_S, numItems, k);
	getUmbrales(umbrales, numItems, conn);

	for(int i = 0; i < numIter; i++){
		cout<<"Iteracion "<<i<<endl;
		cout<<"Calculando P_S"<<endl;
		operateP_S(P_S, Q_S, N_ofItems, umbrales, numISR_ofUsers, regPar);
		cout<<"Calculando Q_S"<<endl;
		operateQ_S(P_S, Q_S, numIR_ofUsers, umbrales, numISR_ofUsers, regPar);
	}

	cout<<"Calculando Transpuesta de Q_S"<<endl<<endl;
	Q_S = trans(Q_S);

	N_ofItems.clear();
	N_ofItems.shrink_to_fit();
	numIR_ofUsers.clear();
	numIR_ofUsers.shrink_to_fit();
	numISR_ofUsers.clear();
	numISR_ofUsers.shrink_to_fit();
	umbrales.clear();
	umbrales.shrink_to_fit();
}




void parallelT_prima(matrix<ValType> * PQ_T, ValType umbralT, int ini, int end){
	mapped_vector<ValType> T;
	std::vector<IndexType> indexVec;
	int indexIzq = 0;
	int indexDer = 0;

	MYSQL * conn = getConnect(BDName);

	for(int i = ini; i < end; i++){
		(*PQ_T)(i,0) = 0;
		getT_user(T, i, PQ_T->size2(), indexVec, conn);
		sort(indexVec.begin(), indexVec.end());
		indexIzq = 0;
		indexDer = 0;
		for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
			indexDer = (*iter);
			(*PQ_T)(i, indexDer) = 1;
			for(int j = indexDer - 1; j > indexIzq; j--){
				if((*PQ_T)(i,j) >= umbralT) (*PQ_T)(i,j) = 1;
				else (*PQ_T)(i,j) = 0;
			}
			indexIzq = indexDer;
		}
		for(int j = PQ_T->size2() - 1; j > indexIzq; j--){
			if((*PQ_T)(i,j) >= umbralT) (*PQ_T)(i,j) = 1;
			else (*PQ_T)(i,j) = 0;
		}
	}

	mapped_vector<ValType>().swap(T);
	indexVec.clear();
	indexVec.shrink_to_fit();
	closeConnect(conn);
}

void UserT_prima(std::vector<ValType> & PQU_T, ValType umbralT, int userId){
	mapped_vector<ValType> T;
	std::vector<IndexType> indexVec;
	int indexIzq = 0;
	int indexDer = 0;

	MYSQL * conn = getConnect(BDName);
	PQU_T[0] = 0;
	getT_user(T, userId, PQU_T.size(), indexVec, conn);
	closeConnect(conn);
	sort(indexVec.begin(), indexVec.end());
	indexIzq = 0;
	indexDer = 0;
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		indexDer = (*iter);
		PQU_T[indexDer] = 1;
		for(int j = indexDer - 1; j > indexIzq; j--){
			if(PQU_T[j] >= umbralT) PQU_T[j] = 1;
			else PQU_T[j] = 0;
		}
		indexIzq = indexDer;
	}
	for(int j = PQU_T.size() - 1; j > indexIzq; j--){
		if(PQU_T[j] >= umbralT) PQU_T[j] = 1;
		else PQU_T[j] = 0;
	}

	mapped_vector<ValType>().swap(T);
	indexVec.clear();
	indexVec.shrink_to_fit();
}


void UserS_prima(std::vector<ValType> & PQU_S, ValType umbralS, std::vector<ValType> & umbrales, int userId){
	mapped_vector<ValType> S;
	std::vector<IndexType> indexVec;
	std::vector<IndexType> indexCeroVec;
	std::vector<tuple<IndexType, bool>> indexAllVec;
	int indexIzq = 0;
	int indexDer = 0;

	MYSQL * conn = getConnect(BDName);

	
	PQU_S[0] = 0;
	indexAllVec.clear();
	indexAllVec.shrink_to_fit();
	getS_user(S, userId, PQU_S.size(), indexVec, indexCeroVec, umbrales, conn);
	closeConnect(conn);
	for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
		indexAllVec.push_back(make_tuple((*iter), 1));
	}
	for(auto iter = indexCeroVec.begin(); iter != indexCeroVec.end(); ++iter){
		indexAllVec.push_back(make_tuple((*iter), 0));
	}
	
	sort(indexAllVec.begin(), indexAllVec.end(), [] (tuple<IndexType,bool> a, tuple<IndexType,bool> b){
		return get<0>(a) < get<0>(b);
	});
	indexIzq = 0;
	indexDer = 0;
	for(auto iter = indexAllVec.begin(); iter != indexAllVec.end(); ++iter){
		indexDer = get<0>(*iter);
		PQU_S[indexDer] = get<1>(*iter);
		for(int j = indexDer - 1; j > indexIzq; j--){
			if(PQU_S[j] >= umbralS) PQU_S[j] = 1;
			else PQU_S[j] = 0;
		}
		indexIzq = indexDer;
	}
	for(int j = PQU_S.size() - 1; j > indexIzq; j--){
		if(PQU_S[j] >= umbralS) PQU_S[j] = 1;
		else PQU_S[j] = 0;
	}
		

	mapped_vector<ValType>().swap(S);
	indexVec.clear();
	indexVec.shrink_to_fit();
	indexCeroVec.clear();
	indexCeroVec.shrink_to_fit();
	indexAllVec.clear();
	indexAllVec.shrink_to_fit();
}

void parallelS_prima(matrix<ValType> * PQ_S, ValType umbralS, std::vector<ValType> * umbrales, int ini, int end){
	mapped_vector<ValType> S;
	std::vector<IndexType> indexVec;
	std::vector<IndexType> indexCeroVec;
	std::vector<tuple<IndexType, bool>> indexAllVec;
	int indexIzq = 0;
	int indexDer = 0;

	MYSQL * conn = getConnect(BDName);

	for(int i = ini; i < end; i++){
		(*PQ_S)(i,0) = 0;
		indexAllVec.clear();
		indexAllVec.shrink_to_fit();
		getS_user(S, i, PQ_S->size2(), indexVec, indexCeroVec, *umbrales, conn);
		for(auto iter = indexVec.begin(); iter != indexVec.end(); ++iter){
			indexAllVec.push_back(make_tuple((*iter), 1));
		}
		for(auto iter = indexCeroVec.begin(); iter != indexCeroVec.end(); ++iter){
			indexAllVec.push_back(make_tuple((*iter), 0));
		}
		
		sort(indexAllVec.begin(), indexAllVec.end(), [] (tuple<IndexType,bool> a, tuple<IndexType,bool> b){
			return get<0>(a) < get<0>(b);
		});
		indexIzq = 0;
		indexDer = 0;
		for(auto iter = indexAllVec.begin(); iter != indexAllVec.end(); ++iter){
			indexDer = get<0>(*iter);
			(*PQ_S)(i,indexDer) = get<1>(*iter);
			for(int j = indexDer - 1; j > indexIzq; j--){
				if((*PQ_S)(i,j) >= umbralS) (*PQ_S)(i,j) = 1;
				else (*PQ_S)(i,j) = 0;
			}
			indexIzq = indexDer;
		}
		for(int j = PQ_S->size2() - 1; j > indexIzq; j--){
			if((*PQ_S)(i,j) >= umbralS) (*PQ_S)(i,j) = 1;
			else (*PQ_S)(i,j) = 0;
		}
	}	

	mapped_vector<ValType>().swap(S);
	indexVec.clear();
	indexVec.shrink_to_fit();
	indexCeroVec.clear();
	indexCeroVec.shrink_to_fit();
	indexAllVec.clear();
	indexAllVec.shrink_to_fit();
	closeConnect(conn);
}

void parallelR_prima(matrix<ValType> * PQ_T, matrix<ValType> * PQ_S, matrix<int> * R_prima,
						std::vector<tuple<int,int>> * R_primaCeros, int ini, int end){

	ValType PQ_T_Val = 0;
	ValType PQ_S_Val = 0;

	for(int i = ini; i < end; i++){
		for(int j = 0; j < R_prima->size2(); j++){
			PQ_T_Val = (*PQ_T)(i,j);
			PQ_S_Val = (*PQ_S)(i,j);
			if(PQ_T_Val == 1 and PQ_S_Val == 1) (*R_prima)(i,j) = ST_VAL;
			else if(PQ_T_Val == 1) (*R_prima)(i,j) = T_VAL;
			else if(PQ_S_Val == 1) (*R_prima)(i,j) = S_VAL;
			else{
				(*R_prima)(i,j) = CERO_VAL;
				R_primaCeros->push_back(make_tuple(i,j));
			} 
		}
	}
}

void UserR_prima(std::vector<ValType> & PQU_T, std::vector<ValType> & PQU_S, std::vector<int> & R_prima){
	R_prima.clear();
	R_prima.shrink_to_fit();
	R_prima = std::vector<int>(PQU_T.size(), 0);
	ValType PQ_T_Val = 0;
	ValType PQ_S_Val = 0;
	for(int j = 1; j < R_prima.size(); j++){
		PQ_T_Val = PQU_T[j];
		PQ_S_Val = PQU_S[j];
		if(PQ_T_Val == 1 and PQ_S_Val == 1) R_prima[j] = ST_VAL;
		else if(PQ_T_Val == 1) R_prima[j] = T_VAL;
		else if(PQ_S_Val == 1) R_prima[j] = S_VAL;
		else{
			R_prima[j] = CERO_VAL;
		} 
	}
}

void candidateDet(matrix<ValType> & PQ_T, matrix<ValType> & PQ_S, matrix<int> & R_prima, 
					std::vector<tuple<int,int>> & R_primaCeros, ValType umbralT, ValType umbralS, MYSQL * conn){
	std::vector<ValType> umbrales;
	getUmbrales(umbrales, PQ_T.size2(), conn);

	cout<<"Generando T_prima"<<endl;
	for(int i = 0; i < PQ_T.size2(); i++){
		PQ_T(0,i) = 0;
	}

	thread threads[NUM_THREADS];
	int h = (PQ_T.size1() - 1) / NUM_THREADS;
	int ini = 1;
	int end = 0;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = PQ_T.size1();
		else end = ini + h;
		threads[i] = thread(parallelT_prima, &PQ_T, umbralT, ini, end);
		ini = end;
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}
	
	cout<<"Generando S_prima"<<endl;
	for(int i = 0; i < PQ_S.size2(); i++){
		PQ_S(0,i) = 0;
	}

	h = (PQ_S.size1() - 1) / NUM_THREADS;
	ini = 1;
	end = 0;
	for(int i = 0; i < NUM_THREADS; i++){
		if(i == NUM_THREADS - 1) end = PQ_T.size1();
		else end = ini + h;
		threads[i] = thread(parallelS_prima, &PQ_S, umbralS, &umbrales, ini, end);
		ini = end;	
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
	}
	umbrales.clear();
	umbrales.shrink_to_fit();

	R_prima = matrix<int>(PQ_T.size1(), PQ_T.size2());

	std::vector<tuple<int,int>> * primaCerosRes[NUM_THREADS];

	cout<<"Generando R_prima"<<endl;

/*
	ini = 1;
	end = 0;
	for(int i = 0; i < NUM_THREADS; i++){
		primaCerosRes[i] = new std::vector<tuple<int,int>>();
		if(i == NUM_THREADS - 1) end = R_prima.size1();
		else end = ini + h;
		threads[i] = thread(parallelR_prima, &PQ_T, &PQ_S, &R_prima, primaCerosRes[i], ini, end);
	}
	for(int i = 0; i < NUM_THREADS; i++){
		threads[i].join();
		R_primaCeros.insert(R_primaCeros.end(), primaCerosRes[i]->begin(), primaCerosRes[i]->end());
	}
*/
	
	ValType PQ_T_Val = 0;
	ValType PQ_S_Val = 0;

	for(int i = 0; i < R_prima.size1(); i++){
		for(int j = 0; j < R_prima.size2(); j++){
			PQ_T_Val = PQ_T(i,j);
			PQ_S_Val = PQ_S(i,j);

			if(PQ_T_Val == 1 and PQ_S_Val == 1) R_prima(i,j) = ST_VAL;
			else if(PQ_S_Val == 1) R_prima(i,j) = S_VAL;	
			else if(PQ_T_Val == 1) R_prima(i,j) = T_VAL;
			else{
				R_prima(i,j) = CERO_VAL;
				R_primaCeros.push_back(make_tuple(i,j));
			} 
		}
	}

	//cout<<R_prima<<endl;

}

/*void matrixInjection(std::vector<tuple<int,int>> & R_primaCeros){
	MYSQL * conn = getConnect(BDName);
	string query = "";
	int limit = 10000;
	int count = 0;
	for(auto iter = R_primaCeros.begin(); iter != R_primaCeros.end(); ++iter){
		if(get<0>(*iter) == 0 or get<1>(*iter) == 0) continue;
		cout<<"Insertando 0 en "<<get<0>(*iter)<<" - "<<get<1>(*iter)<<endl;
		query = "INSERT into ratings values (" + to_string(get<0>(*iter)) + "," + to_string(get<1>(*iter)) + ",0)";
		getQuery(conn, query);
	}
	closeConnect(conn);
}
*/

void matrixInjection(std::vector<tuple<int,int>> & R_primaCeros){
	MYSQL * conn = getConnect(BDName);
	string query = "";
	int limit = 5000;
	int count = 0;
	query = "INSERT into ratings values ";
	for(auto iter = R_primaCeros.begin(); iter != R_primaCeros.end(); ++iter){
		if(get<0>(*iter) == 0 or get<1>(*iter) == 0) continue;
		query += "(" + to_string(get<0>(*iter)) + "," + to_string(get<1>(*iter)) + ",0),";
		count++;
		if(count == limit){
			query.pop_back();
			getQuery(conn, query);
			cout<<"Query Insertada"<<endl;
			count = 0;
			query = "INSERT into ratings values ";
		}
	}
	query.pop_back();
	if(count == 0) {
		closeConnect(conn);	
		return;
	}
	getQuery(conn, query);
	cout<<"Query Insertada"<<endl;
	closeConnect(conn);
}

void matrixInjection(std::vector<int> & R_prima, int userId){
	MYSQL * conn = getConnect(BDName);
	string query = "";
	int limit = 20;
	int count = 0;
	int numRandom = rand() % 10;
	limit = 20 + numRandom;

	query = "INSERT into ratings values ";
	for(int i = 1; i < R_prima.size(); i++){
		if(R_prima[i] != CERO_VAL) continue;
		query += "(" + to_string(userId) + "," + to_string(i) + ",0),";
		count++;
		if(count == limit){
			query.pop_back();
			getQuery(conn, query);
			cout<<"Query Insertada:"<<count<<endl;
			count = 0;
			break;
			query = "INSERT into ratings values ";
		}
	}

	query.pop_back();
	if(count == 0) {
		closeConnect(conn);	
		return;
	}
	getQuery(conn, query);
	cout<<"Query Insertada:"<<count<<endl;
	closeConnect(conn);	
	
}

//CEROS, S, T ,ST
tuple<int, int, int, int> getStats(std::vector<int> R_prima){
	int ceros = 0;
	int ss = 0;
	int ts = 0;
	int sts = 0;
	for(int i = 1; i < R_prima.size(); i++){
		if(R_prima[i] == CERO_VAL) ceros++;
		else if(R_prima[i] == S_VAL) ss++;
		else if(R_prima[i] == T_VAL) ts++;
		else if(R_prima[i] == ST_VAL) sts++;
	}
	return make_tuple(ceros,ss,ts,sts);
}

void savePyQ(matrix<ValType> & P, matrix<ValType> & Q, string fileName){
	ofstream outFile(fileName);
	outFile<<P.size1()<<" "<<Q.size2()<<" "<<P.size2()<<endl;
	for(int i = 0; i < P.size1(); i++){
		for(int j = 0; j < P.size2(); j++){
			outFile<<P(i,j)<<" ";
		}
	}
	for(int i = 0; i < Q.size1(); i++){
		for(int j = 0; j < Q.size2(); j++){
			outFile<<Q(i,j)<<" ";
		}
	}
	outFile.close();
}

void loadPyQ(matrix<ValType> & P, matrix<ValType> & Q, string fileName, int & m, int & n){
	ifstream inFile(fileName);
	m = 0;
	n = 0;
	int k = 0;
	inFile>>m;
	inFile>>n;
	inFile>>k;
	P = matrix<ValType>(m,k);
	Q = matrix<ValType>(k,n);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < k; j++){
			inFile>>P(i,j);
		}
	}
	for(int i = 0; i < k; i++){
		for(int j = 0; j < n; j++){
			inFile>>Q(i,j);
		}
	}
	inFile.close();
}

void saveUmbrales(std::vector<ValType> & umbrales, string fileName){
	ofstream outFile(fileName);
	outFile<<umbrales.size()<<endl;
	for(int i = 0; i < umbrales.size(); i++){
		outFile<<umbrales[i]<<endl;
	}
	outFile.close();
}

void loadUmbrales(std::vector<ValType> & umbrales, string fileName){
	ifstream inFile(fileName);
	int size = 0;
	inFile>>size;
	umbrales = std::vector<ValType>(size, 0);
	for(int i = 0; i < size; i++){
		inFile>>umbrales[i];
	}
	inFile.close();
}

void CSII(int numUsers, int numItems, int k, ValType regPar, int numIter){
	ValType umbralT = 0.5;
	ValType umbralS = 0.5;
	matrix<ValType> PQ_T;
	matrix<ValType> PQ_S;
	matrix<int> R_prima;
	std::vector<tuple<int,int>> R_primaCeros;
	MYSQL * conn = getConnect(BDName);
	rWPMF(PQ_T, PQ_S, numUsers, numItems, k, regPar, numIter, conn);
	candidateDet(PQ_T, PQ_S, R_prima, R_primaCeros, umbralT, umbralS, conn);
	matrixInjection(R_primaCeros);
	closeConnect(conn);
}

void CSII_2(int numUsers, int numItems, int k, ValType regPar, int numIter){
	ValType umbralT = 0.5;
	ValType umbralS = 0.5;
	matrix<ValType> P_T;
	matrix<ValType> Q_T;
	matrix<ValType> P_S;
	matrix<ValType> Q_S;
	std::vector<tuple<int,int>> R_primaCeros;
	MYSQL * conn = getConnect(BDName);
	rWPMF(P_T, Q_T, P_S, Q_S, numUsers, numItems, k, regPar, numIter, conn);
	cout<<"Guardando PQT..."<<endl;
	savePyQ(P_T, Q_T, "PQT");
	cout<<"Guardando PQS..."<<endl;
	savePyQ(P_S, Q_S, "PQS");

	
	std::vector<ValType> PQU_T;
	std::vector<ValType> PQU_S;
	std::vector<int> R_prima;
	std::vector<ValType> umbrales;
	getUmbrales(umbrales, numItems, conn);
	closeConnect(conn);
	int ceros = 0;
	int ss = 0;
	int ts = 0;
	int sts = 0;
	for(int i = 1; i < numUsers; i++){
		cout<<i<<"/"<<numUsers<<endl;
		getPQU(P_T, Q_T, PQU_T, i);
		getPQU(P_S, Q_S, PQU_S, i);
		UserT_prima(PQU_T, umbralT, i);
		UserS_prima(PQU_S, umbralS, umbrales, i);
		UserR_prima(PQU_T, PQU_S, R_prima);
		//matrixInjection(R_prima, i);
		tie(ceros,ss,ts,sts) = getStats(R_prima);
		cout<<ceros<<" "<<ss<<" "<<ts<<" "<<sts<<endl;
		//if(ss != 0) cout<<"AAAAAAAAAAAAAAAAA"<<endl;
	}
}

void CSII_3(string PQTFile, string PQSFile, string umbalesFile, int id, std::vector<int> & R_prima){
	int numUsers = 0;
	int numItems = 0;

	ValType umbralT = 0.5;
	ValType umbralS = 0.5;

	matrix<ValType> P_T;
	matrix<ValType> Q_T;
	matrix<ValType> P_S;
	matrix<ValType> Q_S;

	std::vector<ValType> PQU_T;
	std::vector<ValType> PQU_S;
	std::vector<ValType> umbrales;

	//cout<<"Cargando PQT..."<<endl;
	loadPyQ(P_T, Q_T, PQTFile, numUsers, numItems);
	//cout<<"Cargando PQS..."<<endl;
	loadPyQ(P_S, Q_S, PQSFile, numUsers, numItems);
	//MYSQL * conn = getConnect(BDName);
	//getUmbrales(umbrales, numItems, conn);
	loadUmbrales(umbrales, umbalesFile);
	//closeConnect(conn);
	
	getPQU(P_T, Q_T, PQU_T, id);
	getPQU(P_S, Q_S, PQU_S, id);
	UserT_prima(PQU_T, umbralT, id);
	UserS_prima(PQU_S, umbralS, umbrales, id);
	UserR_prima(PQU_T, PQU_S, R_prima);

	//matrixInjection(R_prima, id);

	int ceros = 0;
	int ss = 0;
	int ts = 0;
	int sts = 0;
	tie(ceros,ss,ts,sts) = getStats(R_prima);
	//cout<<ceros<<" "<<ss<<" "<<ts<<" "<<sts<<endl;
	//if(ss != 0) cout<<"AAAAAAAAAAAAAAAAA"<<endl;
}

void CSII_4(string PQTFile, string PQSFile){
	int numUsers = 0;
	int numItems = 0;

	ValType umbralT = 0.5;
	ValType umbralS = 0.5;

	matrix<ValType> P_T;
	matrix<ValType> Q_T;
	matrix<ValType> P_S;
	matrix<ValType> Q_S;

	std::vector<ValType> PQU_T;
	std::vector<ValType> PQU_S;
	std::vector<int> R_prima;
	std::vector<ValType> umbrales;

	cout<<"Cargando PQT..."<<endl;
	loadPyQ(P_T, Q_T, PQTFile, numUsers, numItems);
	cout<<"Cargando PQS..."<<endl;
	loadPyQ(P_S, Q_S, PQSFile, numUsers, numItems);
	MYSQL * conn = getConnect(BDName);
	cout<<"Generando umbrales.."<<endl;
	getUmbrales(umbrales, numItems, conn);
	closeConnect(conn);
	int ceros = 0;
	int ss = 0;
	int ts = 0;
	int sts = 0;
	for(int i = 1; i < numUsers; i++){
		cout<<i<<"/"<<numUsers<<endl;
		getPQU(P_T, Q_T, PQU_T, i);
		getPQU(P_S, Q_S, PQU_S, i);
		UserT_prima(PQU_T, umbralT, i);
		UserS_prima(PQU_S, umbralS, umbrales, i);
		UserR_prima(PQU_T, PQU_S, R_prima);
		//matrixInjection(R_prima, i);
		tie(ceros,ss,ts,sts) = getStats(R_prima);
		cout<<ceros<<" "<<ss<<" "<<ts<<" "<<sts<<endl;
		//if(ss != 0) cout<<"AAAAAAAAAAAAAAAAA"<<endl;
	}
}

#endif