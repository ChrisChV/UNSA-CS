#include <iostream>
#include <fstream>
#include "csii.h"

using namespace std;

int main(int argc, char ** argv){
	srand(time(NULL));
	if(argc != 3){
		cout<<"Faltan Argumentos <UserID> <db>"<<endl;
		return 0;
	}

	string s_uis(argv[1]);
	string db(argv[2]);
	int uid = stoi(s_uis);

	std::vector<int> R_prima;


	CSII_3("PQ/ml_" + db + "T", "PQ/ml_"+ db + "S", "PQ/umbrales" + db, uid, R_prima);



	std::vector<int> serendipia;

	for(int i = 1; i < R_prima.size(); i++){
		if(R_prima[i] == S_VAL) serendipia.push_back(i);
	}

	

	//for(int i : serendipia){
	//	cout<<i<<endl;
	//}

	MYSQL * conn = getConnect("ml_" + db);
	MYSQL_RES * res = nullptr;
	MYSQL_ROW row;

	int randNumber1 = 0;
	int randNumber2 = 0;

	//ofstream outFile("outSeren");

	if(serendipia.size() == 0){
		cout<<"0";
		//outFile.close();
	}
	else if(serendipia.size() == 1){
		cout<<"1"<<";";
		randNumber1 = rand() % serendipia.size();		
		int mid1 = serendipia[randNumber1];
		res = getQueryRes(conn, "select TMDB_ID, MovieName, MovieId from movies where movieId = " + to_string(mid1));
		row = mysql_fetch_row(res);
		cout<<atoi(row[0])<<";";
		cout<<row[1]<<";";
		cout<<atoi(row[2]);
		//outFile.close();
		mysql_free_result(res);
	}
	else{
		cout<<"2"<<" ";
		randNumber1 = rand() % serendipia.size();
		randNumber2 = rand() % serendipia.size();
		if(randNumber2 == randNumber1) randNumber2 = (randNumber2 + 1) % serendipia.size();

		int mid1 = serendipia[randNumber1];
		int mid2 = serendipia[randNumber2];	

		res = getQueryRes(conn, "select TMDB_ID, MovieName, MovieId from movies where movieId = " + to_string(mid1));
		row = mysql_fetch_row(res);
		cout<<atoi(row[0])<<";";
		cout<<row[1]<<";";
		cout<<atoi(row[2])<<";";
		//outFile.close();
		mysql_free_result(res);

		res = getQueryRes(conn, "select TMDB_ID, MovieName, MovieId from movies where movieId = " + to_string(mid2));
		row = mysql_fetch_row(res);
		cout<<atoi(row[0])<<";";
		cout<<row[1]<<";";
		cout<<atoi(row[2]);
		//outFile.close();
		mysql_free_result(res);
	}
	closeConnect(conn);
}