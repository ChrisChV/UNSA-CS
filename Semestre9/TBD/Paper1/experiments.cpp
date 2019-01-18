#include <iostream>
#include <fstream>
#include <tuple>
#include "connector.h"
#include "FileOp.h"
#include "MyTime.h"

int main(){
	
	auto bdInter = getBd("../ml/ml-1m/ratings.dat", ';');

	cout<<"Generando vals"<<endl;
	vector<tuple<int,int>> userIds;

	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		userIds.push_back(make_tuple(stoi((*iter)[0]), stoi((*iter)[1])));
	}

	int numUsers = 6040;

	MYSQL * conn2;
	MYSQL_ROW row1;
	MYSQL_ROW row2;
	MYSQL_RES * res1 = nullptr;
	MYSQL_RES * res2 = nullptr;
	double res = 0;

	ofstream outFile("expOut4");

	int i = 0;
	int mid = 0;
	for(auto tt : userIds){
		i = get<0>(tt);
		mid = get<1>(tt);
		if(i < 1) continue;
		cout<<i<<"/"<<numUsers<<" -> "<<mid<<endl;
		conn2 = getConnect("ml_1m");
		res2 = getQueryRes(conn2, "call recommendCosTest(" + to_string(i) + "," + to_string(mid) + ")");
		closeConnect(conn2);
		if((row2 = mysql_fetch_row(res2))){
			res = atof(row2[3]);
			outFile<<i<<" "<<mid<<" "<<res<<endl;
		}
		mysql_free_result(res2);
		
	}
	
	outFile.close();

/*
	for(auto tt : userIds){
		i = get<0>(tt);
		mid = get<1>(tt);
		cout<<i<<"/"<<numUsers<<endl;
		string query1 = "select MovieId from ratings where userId = " + to_string(i);
		res1 = getQueryRes(conn, query1);
		while((row1 = mysql_fetch_row(res1))){
			conn2 = getConnect("ml");
			res2 = getQueryRes(conn2, "call recommendSlopeTest(" + to_string(i) + "," + to_string(atoi(row1[0])) + ")");
			closeConnect(conn2);
			row2 = mysql_fetch_row(res2);
			res = atof(row2[3]);
			outFile<<i<<" "<<atoi(row1[0])<<" "<<res<<endl;
			mysql_free_result(res2);
		}
		mysql_free_result(res1);
	}
	
	outFile.close();
	closeConnect(conn);
*/
}
