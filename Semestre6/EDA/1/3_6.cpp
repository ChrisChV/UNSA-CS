#include <iostream>

using namespace std;

namespace DataStructures{

	template<typename T>
	class CLinkedList{
		private:
			struct NODE{
				T m_data;
				struct NODE * m_next;
			};
			struct NODE * m_root;
		public:
			CLinkedList(){m_root = nullptr;};
			void insert(T elem);
			void print();
	};

	template<typename T>
	void CLinkedList<T>::insert(T elem){
		struct NODE * node = new NODE;
		node->m_data = elem;
		node->m_next = nullptr;
		if(m_root == nullptr) {
			m_root = node;
			return;
		}
		struct NODE * temp = m_root;
		while(temp->m_next != nullptr){
		 	temp = temp->m_next;
		}
		temp->m_next = node;
	}

	template<typename T>
	void CLinkedList<T>::print(){
		struct NODE * temp = m_root;
		while(temp != nullptr){
			cout<<temp->m_data<<"->";
			temp = temp->m_next;
		}
		cout<<endl;
	}


	template<typename T>
	class CBinaryTree{
		private:
			struct NODE{
				T m_data;
				struct NODE * hijos[2];
			};
			struct NODE * m_root;
			void _print(NODE * node);
		public:
			CBinaryTree(){m_root = nullptr;};
			void insert(T elem);
			void print();
	};

	template<typename T>
	void CBinaryTree<T>::insert(T elem){
		struct NODE * node = new NODE;
		node->m_data = elem;
		node->hijos[0] = nullptr;
		node->hijos[1] = nullptr;
		if(m_root == nullptr){
			m_root = node;
			return;
		}
		struct NODE * temp = m_root;
		bool flag;
		while(temp->hijos[flag = (elem > temp->m_data)] != nullptr){
			temp = temp->hijos[flag];
		}
		temp->hijos[flag] = node;
	}

	template<typename T>
	void CBinaryTree<T>::print(){
		_print(m_root);
		cout<<endl;
	}

	template<typename T>
	void CBinaryTree<T>::_print(NODE * node){
		if(node == nullptr) return;
		_print(node->hijos[0]);
		cout<<node->m_data<<"->";
		_print(node->hijos[1]);
	}

};

int main(){
	DataStructures::CLinkedList<int> myList;
	DataStructures::CBinaryTree<int> myTree;
	for(int i = 0; i < 20; i++){
		myList.insert(i);
		myTree.insert(i);
	}
	myList.print();
	myTree.print();
}