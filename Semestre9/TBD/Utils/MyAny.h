#ifndef MYANY_H
#define MYANY_H

#include <iostream>
#include <boost/any.hpp>

using namespace std;

class MyAny{
	public:
		MyAny(){}
		MyAny(boost::any val, int type_id);
	private:
		boost::any val;
		int type_id;
};

MyAny::MyAny(boost::any val, int type_id){
	this->val = val;
	
}

#endif