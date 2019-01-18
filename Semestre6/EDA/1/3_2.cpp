#include <iostream>

using namespace std;

struct Vector{
	int *m_pVect;
	int m_nCount;
	int m_nMax;
	int m_nDelta;
};

void print(int *& rpVect, int &rnCount){
	for(int i = 0; i < rnCount; i++){
		cout<<rpVect[i]<<"->";
	}
	cout<<endl;
}

void Resize(Vector *pThis){
	pThis->m_pVect = (int*) realloc(pThis->m_pVect, sizeof(int) * (pThis->m_nMax + pThis->m_nDelta));
	pThis->m_nMax += pThis->m_nDelta;
}

void Insert(Vector *pThis, int elem){
	if(pThis->m_nCount == pThis->m_nMax){
		Resize(pThis);
	}
	pThis->m_pVect[pThis->m_nCount++] = elem;
}

void Resize(int *& rpVect, int & rnMax){
	const int delta = 2;
	rpVect = (int*) realloc(rpVect, sizeof(int) * (rnMax + delta));
	rnMax += delta;
}

void Insert(int *& rpVect, int &rnCount, int &rnMax, int elem){
	if(rnCount == rnMax){
		Resize(rpVect,rnMax);
	}
	rpVect[rnCount++] = elem;
}

int main(){
	int *vec1 = nullptr;
	int count = 0;
	int max = 0;
	

	struct Vector vec2;
	vec2.m_pVect = nullptr;
	vec2.m_nCount = 0;
	vec2.m_nMax = 0;
	vec2.m_nDelta = 2;

	for(int i = 0; i < 20; i++){
		Insert(vec1,count,max,i);
		Insert(&vec2,i);
	}

	print(vec1,count);
	print(vec2.m_pVect,vec2.m_nCount);

}