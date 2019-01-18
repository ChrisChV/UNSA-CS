#include <iostream>
//#include <mysql.h>
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

using namespace std;
using namespace boost::numeric::ublas;

//MYSQL * conn;

bool invertMatrix(mapped_matrix<double> & input, mapped_matrix<double> & inversa){
	mapped_matrix<double> A(input);
	permutation_matrix<size_t> pm(A.size1());
	int res = lu_factorize(A,pm);
	if(res != 0) return false;

	inversa.assign(identity_matrix<double>(A.size1()));

	lu_substitute(A,pm,inversa);

	return true;
}


int main(){

	mapped_vector<double> v1(2);
	v1[0] = 1;
	v1[1] = 2;


	mapped_matrix<double> m1(2, 3);
	m1(0,0) = 1;
	m1(0,1) = 2;
	m1(0,2) = 3;
	m1(1,0) = 4;
	m1(1,1) = 5;
	m1(1,2) = 6;

	mapped_vector<double> res(3);

	axpy_prod(v1, m1, res, true);

	cout<<res<<endl;

/*
	mapped_vector<double> vec(4);
	vec[0] = 3;
	vec[1] = 2;
	vec[2] = 3;
	vec[3] = 1;

	mapped_matrix<double> mtr(4,4);
	mtr(0,0) = 4;
	mtr(1,1) = 3;
	mtr(2,2) = 2;
	mtr(3,3) = 1;

	mapped_vector<double> res(4);

	//for(auto iter = vec.begin(); iter != vec.end(); ++iter){
//		(*iter) = 1;
//	}

	mapped_matrix<double> mtr2(4,2);
	mtr2(0,0) = 1;
	mtr2(1,1) = 2;
	mtr2(1,1) = 2;
	mtr2(2,1) = 3;
	mtr2(3,0) = 4;

	cout<<mtr2<<endl;

	mtr2 += mtr2;

	cout<<mtr2<<endl;
*/

/*

	cout<<mtr<<endl;

	mtr *= 2;

	cout<<mtr<<endl;

	
	mapped_vector<double>(4).swap(vec);

	cout<<vec<<endl;

	identity_matrix<double> mi(3);
	mapped_matrix<double> mii(mi);

	cout<<mii<<endl;

	mii *= 2;

	cout<<mii<<endl;

	mapped_matrix<double>().swap(mii);
	mii = mapped_matrix<double>(mi);

	cout<<mii<<endl;
*/

/*
	cout<<vec<<endl;
	cout<<mtr<<endl;

	axpy_prod(mtr,vec,res,true);

	cout<<res<<endl;


	mapped_matrix<double> mtr2(4,2);
	mtr2(0,0) = 1;
	mtr2(1,1) = 2;
	mtr2(1,1) = 2;
	mtr2(2,1) = 3;
	mtr2(3,0) = 4;

	cout<<mtr2<<endl;

	mapped_matrix<double> mtr2Trans = trans(mtr2);

	cout<<mtr2Trans<<endl;

	mapped_matrix<double> mtrInversa(2,2);

	mapped_matrix<double> mtr3(2,2);
	mtr3(0,0) = 1;
	mtr3(0,1) = 1;
	mtr3(1,0) = 1;
	mtr3(1,1) = 1;

	cout<<invertMatrix(mtr3, mtrInversa)<<endl;

	cout<<mtrInversa<<endl;
*/
/*
	default_random_engine generator;
  	normal_distribution<double> distribution(0,0.01);
  	generator.seed(time(NULL));

  	for(int i = 0; i < 10; i++){
  		double number = distribution(generator);
  		cout<<number<<endl;
  	}
  	*/

	//identity_matrix<double> m (3);
    //cout<<m<<endl;

	/*
	MYSQL_RES * res;
	MYSQL_ROW row;
	conn =  mysql_init(NULL);
	mysql_real_connect(conn, "localhost", "root", "root", "Test", 3306, NULL, 0);
	mysql_query(conn, "INSERT INTO testTable(puntaje) values (4),(5)");
	mysql_query(conn, "SELECT * from testTable");
	res = mysql_store_result(conn);

	int numFields = mysql_num_fields(res);
	while((row = mysql_fetch_row(res))){
		for(int i = 0; i < numFields; i++){
			if(row[i] != NULL) cout<<atoi(row[i]) + 1<<" ";
			else cout<<"NULL ";
		}
		cout<<endl;
	}
	*/
	
	return 0;
}

