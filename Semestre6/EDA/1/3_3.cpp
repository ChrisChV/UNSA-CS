#include <iostream>

using namespace std;

class CVector{
	private:
		int *m_pVect;
		int m_nCount;
		int m_nMax;
		int m_nDelta;
		void Init(int delta);
		void Resize();
	public:
		CVector(int delta = 10){
			Init(delta);
		}
		void Insert(int elem);
		void print();

};

void CVector::Init(int delta){
	m_pVect = nullptr;
	m_nCount = 0;
	m_nMax = 0;
	m_nDelta = delta;
}

void CVector::Insert(int elem){
	if(m_nCount == m_nMax){
		Resize();
	}
	m_pVect[m_nCount++] = elem;
}

void CVector::Resize(){
	m_pVect = (int *) realloc(m_pVect, sizeof(int) * (m_nMax + m_nDelta));
	m_nMax += m_nDelta;
}

void CVector::print(){
	for(int i = 0; i < m_nCount; i++){
		cout<<m_pVect[i]<<"->";
	}
	cout<<endl;
}

int main(){
	CVector vec(2);
	for(int i = 0; i < 20; i++){
		vec.Insert(i);
	}
	vec.print();
}