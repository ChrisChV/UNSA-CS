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
		class iterator{
			public:
				iterator(int i, CVector * vec);
				Type& operator*(){return (*vec)[index];};				
				bool operator==(iterator iter){return this->index == iter.index;};
				bool operator!=(iterator iter){return this->index != iter.index;};
				void operator++();
				void operator--();
			private:
				int index;
				CVector * vec;
		};
		CVector(int delta = 10){
			Init(delta);
		}
		Type& operator[](int i){return m_pVect[i];};
		void Insert(Type elem);
		void print();
		iterator begin();
		iterator end();
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

template<typename Type>
CVector<Type>::iterator::iterator(int i, CVector * v){
	index = i;
	vec = v;
}

template<typename Type>
typename CVector<Type>::iterator CVector<Type>::begin(){
	if(m_nMax == 0) return iterator(-1,this);
	return iterator(0,this);
}


template<typename Type>
typename CVector<Type>::iterator CVector<Type>::end(){
	return iterator(-1,this);
}

template<typename Type>
void CVector<Type>::iterator::operator++(){
	if(index != -1){
		if(index + 1 == vec->m_nCount) index = -1;
		else index++;
	}
}

template<typename Type>
void CVector<Type>::iterator::operator--(){
	if(index != -1){
		index--;
	}
	else index = vec->m_nCount - 1;
}







template<typename Type>
class CMyComplexDataStructure{
	public:
		CVector<Type> m_container;
		void Insert(Type elem){m_container.Insert(elem);};
		template <typename objclass>
		void sumone(objclass funobj){
			for(auto iter = m_container.begin(); iter != m_container.end(); ++iter){
				funobj(*iter);
			}
		}
};

template<typename Type>
class funobjclass{
	public:
		void operator()(Type & objinstance){
			objinstance++;
		}
};






int main(){
	CMyComplexDataStructure<int> ds;
	for(int i = 0; i < 20; i++){
		ds.Insert(i);
	}
	funobjclass<int> x;
	ds.sumone(x);
	for(auto iter =  ds.m_container.begin(); iter != ds.m_container.end(); ++iter){
		cout<<*iter<<endl;
	}
}


