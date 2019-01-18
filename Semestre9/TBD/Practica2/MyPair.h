#ifndef MYPAIR_H
#define MYPAIR_H

#include <iostream>

using namespace std;

template <typename IDType>
class MyPair{
	public:
		MyPair(){}
		MyPair(IDType id1, IDType id2){
			this->id1 = id1;
			this->id2 = id2;
		}
		
		IDType id1;
		IDType id2;
};

template <typename IDType>
bool operator ==(MyPair<IDType> a, MyPair<IDType> b){
	if(a.id1 == b.id1 and a.id2 == b.id2) return true;
	if(a.id1 == b.id2 and a.id2 == b.id1) return true;
	return false;
}

template <typename IDType>
bool operator >(MyPair<IDType> a, MyPair<IDType> b){
	if(a.id1 > b.id1) return true;
	if(a.id1 == b.id1 and a.id2 > b.id2)  return true;
	return false;
}

template <typename IDType>
bool operator >=(MyPair<IDType> a, MyPair<IDType> b){
	if(a.id1 >= b.id1) return true;
	return false;
}

template <typename IDType>
bool operator <(MyPair<IDType> a, MyPair<IDType> b){
	if(a.id1 < b.id1) return true;
	if(a.id1 == b.id1 and a.id2 < b.id2) return true;
	return false;
}

template <typename IDType>
bool operator <=(MyPair<IDType> a, MyPair<IDType> b){
	if(a.id1 <= b.id1) return true;
	return false;
}


#endif