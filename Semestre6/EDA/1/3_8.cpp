#include <iostream>
#include <string.h>
#include "CVector.h"


using namespace std;

enum TYPES{INT_TYPE,CHAR_TYPE};

class CVectorVoid{
	private:
		void **m_pVect;
		int m_nCount;
		int m_nMax;
		int m_nDelta;
		int m_nElemSize;
		int (*m_lpfnCompare)(void*,void*);
		void Init(int delta);
		void Resize();
		void * DupBlock(void *pElem);
	public:
		CVectorVoid(int (*lpfnCompare)(void*,void*), int nElemSize, int delta =10);
		void * operator[](int i){return m_pVect[i];};
		void Insert(void *elem);
		int compare(void * a, void * b);
};

void CVectorVoid::Init(int delta){
	m_pVect = nullptr;
	m_nCount = 0;
	m_nMax = 0;
	m_nDelta = delta;
}

CVectorVoid::CVectorVoid(int (*lpfnCompare)(void*,void*), int nElemSize, int delta){
	Init(delta);
	m_lpfnCompare = lpfnCompare;
	m_nElemSize = nElemSize;	
}

void CVectorVoid::Insert(void *pElem){
	if(m_nCount == m_nMax){
		Resize();
	}
	m_pVect[m_nCount++] = DupBlock(pElem);
}

void CVectorVoid::Resize(){
	//m_pVect = (void**) realloc(m_pVect, m_nElemSize * (m_nMax + m_nDelta));
	void **temp;
	temp = new void*[m_nMax+m_nDelta];
	for(int i = 0; i < m_nMax; i++){
		temp[i] = m_pVect[i];
	}
	delete [] m_pVect;
	m_pVect = temp;
	m_nMax += m_nDelta;
}

void * CVectorVoid::DupBlock(void * pElem){
	void *p = new char[m_nElemSize];
	return memcpy(p,pElem,m_nElemSize);
}

int CVectorVoid::compare(void * a, void * b){
	return m_lpfnCompare(a,b);
}

void usingCase(){
	int option;
	cout<<"Ingrese su opcion (0:int,1:char)->";
	cin>>option;
	switch(option){
		case INT_TYPE:
		{
			CVector<int> vecInt;
			for(int i = 0; i < 20; i++) vecInt.Insert(i);
			vecInt.print();
			break;
		}
		case CHAR_TYPE:
		{
			CVector<char> vecChar;
			for(int i = 0; i < 20; i++) vecChar.Insert(97 + i);
			vecChar.print();
		}
	}

}


int fnIntCompare(void *pVar1, void *pVar2){
	return *(int *)pVar1 - *(int *)pVar2;
}


int main(){
	CVectorVoid vec(fnIntCompare, sizeof(int), 1);
	
	for(int i = 0; i < 20; i++){
		vec.Insert((void*) &i);
	}

	for(int i = 0; i < 20; i++){
		int temp = *(int*) vec[i];
		cout<<temp<<endl;
	}
	
	int a = 1;
	int b = 2;
	cout<<vec.compare((void*)&a,(void*)&b)<<endl;
	cout<<vec.compare((void*)&b,(void*)&a)<<endl;
	cout<<vec.compare((void*)&a,(void*)&a)<<endl;

/*
	int a = 0;
	int b = 1;
	int c = 2;
	int d = 2;
	int e = 3;
	vec.Insert((void *) &a);
	vec.Insert((void *) &b);
	vec.Insert((void *) &c);
	vec.Insert((void *) &d);
	vec.Insert((void *) &e);

	int aa = *(int*) vec[0];
	int bb = *(int*) vec[1];
	int cc = *(int*) vec[2];
	int dd = *(int*) vec[3];
	cout<<aa<<endl;
	cout<<bb<<endl;
	cout<<cc<<endl;
	cout<<dd<<endl;
*/
}