#include <iostream>

using namespace std;

template <typename Type>
class CVector{
	private:
		Type *m_pVect;
		int m_nCount;
		int m_nMax;
		int m_nDelta;
		void Init(int delta);
		void Resize();
	public:
		CVector(int delta = 10){
			Init(delta);
		}
		void Insert(Type elem);
		void print();

};

template<typename Type>
void CVector<Type>::Init(int delta){
	m_pVect = nullptr;
	m_nCount = 0;
	m_nMax = 0;
	m_nDelta = delta;
}

template<typename Type>
void CVector<Type>::Insert(Type elem){
	if(m_nCount == m_nMax){
		Resize();
	}
	m_pVect[m_nCount++] = elem;
}

template<typename Type>
void CVector<Type>::Resize(){
	m_pVect = (Type *) realloc(m_pVect, sizeof(Type) * (m_nMax + m_nDelta));
	m_nMax += m_nDelta;
}

template<typename Type>
void CVector<Type>::print(){
	for(int i = 0; i < m_nCount; i++){
		cout<<m_pVect[i]<<"->";
	}
	cout<<endl;
}


int main(){
	CVector<int> vec1(2);
	CVector<char> vec2(2);
	for(int i = 0; i < 20; i++){
		vec1.Insert(i);
		vec2.Insert(97 + i);
	}
	vec1.print();
	vec2.print();
}