#include <iostream>
#include "csii.h"

using namespace std;



int main(){
	int numUsers = 0;
	int numItems = 0;
	int k = 3;
	ValType parametroRegularizacion = 0.1;
	int numIter = 3;

	MYSQL * conn = getConnect(BDName);
	MYSQL_RES * res;
	MYSQL_ROW row;
	string query1 = "select UserID from ratings group by UserID order by userId DESC limit 1";
	res = getQueryRes(conn, query1);
	row = mysql_fetch_row(res);
	numUsers = atoi(row[0]) + 1;
	mysql_free_result(res);

	string query2 = "select MovieId from movies order by movieId DESC limit 1";
	res = getQueryRes(conn, query2);
	row = mysql_fetch_row(res);
	numItems = atoi(row[0]) + 1;
	mysql_free_result(res);
	closeConnect(conn);

	//CSII(numUsers, numItems, k, parametroRegularizacion, numIter);
	CSII_2(numUsers, numItems, k, parametroRegularizacion, numIter);
	//CSII_3("PQT","PQS",670);
	//CSII_4("PQ/3T", "PQ/3S");
}