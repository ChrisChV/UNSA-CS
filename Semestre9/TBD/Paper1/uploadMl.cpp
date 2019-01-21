#include <iostream>
#include "connector.h"
#include "FileOp.h"
#include "MyTime.h"

using namespace std;

MYSQL * conn;

// ml 671
// ml-1 6040
// ml-10 71567
// ml-latest 270896


typedef int MovieId;
typedef int TMDBId;
typedef string MovieName;
typedef string MovieCategories;
typedef tuple<MovieName, MovieCategories> MovieRegister;

typedef int UserId;
typedef ValType Valoration;
typedef tuple<MovieId, Valoration> InterRegister;

typedef map<MovieId, Valoration> InterRegisterMap;

MyTime mytime;

void createBD(string bd){
	string query = "CREATE DATABASE IF NOT EXISTS " + bd;
	if(mysql_query(conn, query.c_str()) != 0){
		cout<<"Error al crear base de datos"<<endl;
	}
}

void insertRecomFlags(){
	string query = "select userId from ratings group by userID";
	MYSQL_RES * res = getQueryRes(conn, query);
	MYSQL_ROW row;
	string insertQuery = "insert into RecomFlag values ";
	int number = 0;
	while((row = mysql_fetch_row(res))){
		number = atoi(row[0]);
		insertQuery += "(" + to_string(number) + ",0),";
	}
	insertQuery.pop_back();
	getQuery(conn, insertQuery);
	mysql_free_result(res);
}

void insertMovies(string bd){
	UserId id = 0;
	auto bdMovies = getBd("../ml/ml-1m/movies.dat", ':');
	cout<<"Generando estructura Movies..."<<endl;
	mytime.init();
	map<MovieId, MovieRegister> movieVec;
	for(auto iter = bdMovies.begin(); iter != bdMovies.end(); ++iter){
		id = stoi((*iter)[0]);
		movieVec[id] = make_tuple((*iter)[1], (*iter)[2]);
	}
	mytime.end();
	cout<<"Done->";
	mytime.print();

	auto linksMovies = getBd("../ml/" + bd + "/links.csv", ',');

	map<MovieId, TMDBId> tmdbIds;
	for(auto iter = linksMovies.begin(); iter != linksMovies.end(); ++iter){
		id = stoi((*iter)[0]);
		if((*iter)[2].front() == 13){
			tmdbIds[id] = -1;
		}
		else{
			tmdbIds[id] = stoi((*iter)[2]);
		}
	}


	string tableQuery = "CREATE TABLE IF NOT EXISTS movies(MovieId INT NOT NULL AUTO_INCREMENT, MovieLensId INT NOT NULL, MovieName VARCHAR(256), TMDB_ID INT NOT NULL, PRIMARY KEY(MovieId)) ";
	if(mysql_query(conn, tableQuery.c_str()) != 0){
		cout<<"Error al crear table de peliculas"<<endl;
		return;
	}
	int count = 0;
	//string q = "set root utf8";
	//if(mysql_query(conn, q.c_str()) != 0){
	//	cout<<"Error al insertar peliculas"<<endl;
	//	return;
	//}
	string query = "INSERT INTO movies(MovieLensId,MovieName, TMDB_ID) VALUES ";
	for(auto iter = movieVec.begin(); iter != movieVec.end(); ++iter){
		query += "(" + to_string(iter->first) + ",'" + get<0>(iter->second) + "'," + to_string(tmdbIds[iter->first]) + "),";
		count++;
		if(count == 1000){
			query.pop_back();
			if(mysql_query(conn, query.c_str()) != 0){
				cout<<"Error al insertar peliculas"<<endl;
				return;
			}
			cout<<"Query insertada"<<endl;
			query = "INSERT INTO movies(MovieLensId,MovieName, TMDB_ID) VALUES ";
			count = 0;
		}
	}
	query.pop_back();
	if(mysql_query(conn, query.c_str()) != 0){
		cout<<"Error al insertar peliculas"<<endl;
		return;
	}
	cout<<"Query insertada"<<endl;
}

void _insertInters(string bd, int numUsers, int left_limit, int rigth_limit, map<MovieId,MovieId> & movieMap){
	cout<<"Levantando BD"<<endl;
	auto bdInter = getBd("../ml/ml-1m/ratings.dat", ';', left_limit, rigth_limit);


	cout<<"Generando vals"<<endl;
	map<int, InterRegisterMap> valsByUser;
	Valoration val = 0;	

	UserId id = 0;
	MovieId id2 = 0;

	for(auto iter = bdInter.begin(); iter != bdInter.end(); ++iter){
		id = stoi((*iter)[0]);
		id2 = stoi((*iter)[1]);
		val = stof((*iter)[2]);
		valsByUser[id][id2] = val;
	}

	//Tenemos que ver si lo ordenamos por usuario o por pelicula

	string tableQuery =  "CREATE TABLE IF NOT EXISTS ratings(UserId INT NOT NULL, MovieId INT NOT NULL, PUNTAJE FLOAT, PRIMARY KEY(UserId,MovieId)	)";
	//if(mysql_query(conn, tableQuery.c_str()) != 0){
	//	cout<<"Error al crear table de ratings"<<endl;
	//	return;
	//}

	MYSQL_RES * res;
	MYSQL_ROW row;

	string query = "";

	InterRegisterMap::iterator findRes;
	string actualValue = "";

	bdInter.clear();
	bdInter.shrink_to_fit();

	int count = 0;

	/*
	for(int i = 1; i < valsByUser.size(); i++){
		query = "INSERT INTO ratings VALUES ";
		for(auto iter = movieMap.begin(); iter != movieMap.end(); ++iter){
			findRes = valsByUser[i].find(iter->first);
			if(findRes == valsByUser[i].end()) continue;
			query += "(" + to_string(i) + "," + to_string(iter->second) + "," + to_string(findRes->second) + "),";
			count++;
		}
		query.pop_back();
		if(query == "INSERT INTO ratings VALUES") continue;
		if(mysql_query(conn, query.c_str()) != 0){
			cout<<"Error al insertar"<<endl;
			return;
		}
		cout<<"Query insertada -> "<<count<<endl;
		if(count + 1 == rigth_limit - left_limit) break;

	}
	*/

	int i = 0;
	for(auto iiter = valsByUser.begin(); iiter != valsByUser.end(); ++iiter){
		i = iiter->first;
		query = "INSERT INTO ratings VALUES ";
		for(auto iter = movieMap.begin(); iter != movieMap.end(); ++iter){
			findRes = valsByUser[i].find(iter->first);
			if(findRes == valsByUser[i].end()) continue;
			query += "(" + to_string(i) + "," + to_string(iter->second) + "," + to_string(findRes->second) + "),";
			count++;
		}
		query.pop_back();
		if(query == "INSERT INTO ratings VALUES") continue;
		if(mysql_query(conn, query.c_str()) != 0){
			cout<<"Error al insertar"<<endl;
			//return;
		}
		cout<<"Query insertada -> "<<count<<endl;
		if(count + 1 == rigth_limit - left_limit) break;

	}

	bdInter.clear();
	bdInter.shrink_to_fit();
	map<int, InterRegisterMap>().swap(valsByUser);
}

void insertInters(string bd, int numUsers, int numReg, int actual, map<MovieId,MovieId> & movieMap){
	int h = 10000;
	int left_limit = 0;
	int rigth_limit = 0;
	left_limit = h * actual;
	rigth_limit = left_limit + h;
	while(true){
		_insertInters(bd,  numUsers, left_limit,  rigth_limit, movieMap);
		if(rigth_limit > numReg) break;
		cout<<"----------INGRESADO -> "<<actual<<" ----------------"<<endl;
		actual++;
		left_limit = rigth_limit;
		rigth_limit += h;
	}
}

int main(int argc, char ** argv){
	if(argc != 3){
		cout<<"Faltan Argumentos <BD> <numUsers>"<<endl;
		return 0;
	}
	
	string bdName(argv[1]);
	string sNumUsers(argv[2]);
	int numUsers = stoi(sNumUsers);

	MYSQL_RES * res;
	MYSQL_ROW row;

	conn = getConnect(bdName);

	//cout<<"Hello"<<endl;


	string movieQuery = "SELECT MovieId, MovieLensId from movies";
	if(mysql_query(conn, movieQuery.c_str()) != 0){
		cout<<"Error al leer tabla movies"<<endl;
		return 0;
	}

	cout<<"Generando Movie Map"<<endl;
	map<MovieId,MovieId> movieMap;

	res = mysql_store_result(conn);
	int numFields = mysql_num_fields(res);
	while((row = mysql_fetch_row(res))){
		if(row[0] == NULL or row[1] == NULL){
			cout<<"Valor nulo"<<endl;
			return 0;
		}
		movieMap[atoi(row[1])] = atoi(row[0]);
	}
	
	

	//int numReg = 26024289;
	//int actual = 1669;

	int numReg = 1000209;
	int actual = 0;
	numUsers = 6040;


	//createBD(bdName);
	//insertMovies(bdName);
	insertInters(bdName, numUsers, numReg, actual, movieMap);
	//insertRecomFlags();
	closeConnect(conn);

	//system("shutdown -h now");
	return 0;
}
