#ifndef SATREE_H
#define SATREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

typedef float Dis;
typedef int Dimension;
typedef int ID;

template <typename kPoint>
class SATree{
	private:
		
		class Node;

	public:

		typedef Dis (*FunDis)(kPoint,kPoint);
		typedef vector<Node*> SetOfNodes;
		typedef vector<kPoint> SetOfKPoints;

		SATree(){
			distance = nullptr;
			root = nullptr;			
			idActual = 0;
			dim = 0;
		}
		SATree(FunDis dis, SetOfKPoints S){
			idActual = 0;
			dim = 0;
			distance = dis;
			buildTree(S);
		}
		void buildTree(SetOfKPoints S);
		void print(string file);

	private:

		class Node{
			public:
				Node(){
					id = -1;
				}
				Node(kPoint val, ID id){
					this->val = val;
					this->id = id;
					init();
				}
				void init(){
					neighbors.clear();
					coverRadio = 0;
					disToNode = 0;
				};
				kPoint getVal(){return val;}
				void setDisToNode(Dis dis){disToNode = dis;}
				void setCoverRadio(Dis radio){coverRadio = radio;}
				void addNeighbors(Node * node){neighbors.push_back(node);}
				void addSubTree(Node * node){subTrees.push_back(node);}
				void resetSubTrees(){subTrees.clear();}
				Dis getDisToNode(){return disToNode;}
				Dis getCoverRadio(){return coverRadio;}
				SetOfNodes getNeighbors(){return neighbors;}
				SetOfNodes & getSubTrees(){return subTrees;}
				ID getId(){return id;}
			private:
				kPoint val;
				ID id;
				SetOfNodes neighbors;
				SetOfNodes subTrees;
				Dis coverRadio;
				Dis disToNode;
		};

		Dimension dim;
		FunDis distance;
		Node * root;
		ID idActual;

		void _buildTree(Node * actual, SetOfNodes & set);
		//bool _funSortByDistance(Node * a, Node * b);
		Dis _sortByDistance(Node * actual, SetOfNodes & set);
};

template <typename kPoint>
void SATree<kPoint>::buildTree(SetOfKPoints setKPoints){
	srand(time(NULL));
	int randNumber = rand() % setKPoints.size();
	SetOfNodes set;
	root = new Node(setKPoints[randNumber], idActual);
	idActual++;
	setKPoints.erase(setKPoints.begin() + randNumber);
	for(kPoint point : setKPoints){
		set.push_back(new Node(point,idActual));
		idActual++;
	}

	_buildTree(root,set);
}

//template <typename kPoint>
//bool _funSortByDistance(SATree<kPoint>::Node * a, SATree<kPoint>::Node * b){
//	return a->getDisToNode() < b->getDisToNode();
//}

template <typename kPoint>
Dis SATree<kPoint>::_sortByDistance(Node * actual, SetOfNodes & set){
	for(Node * n : set){
		n->setDisToNode(distance(n->getVal(),actual->getVal()));
	}
	sort(set.begin(),set.end(),[](Node * a, Node * b){
		return a->getDisToNode() < b->getDisToNode();
	});
}

template <typename kPoint>
void SATree<kPoint>::_buildTree(Node * actual, SetOfNodes & set){
	if(set.size() == 0 or actual == nullptr) return;
	actual->init();
	_sortByDistance(actual, set);
	
	for(auto iter = set.begin(); iter != set.end(); ++iter
		Node * v = *iter;
		Dis tt = max(actual->getCoverRadio(),v->getDisToNode());
		actual->setCoverRadio(tt);
		if(actual->getNeighbors().size() == 0){
			actual->addNeighbors(v);
			set.erase(iter);
			--iter;
		}
		else{
			bool flag = true;
			for(Node * b : actual->getNeighbors()){
				if(!(v->getDisToNode() < distance(v->getVal(),b->getVal()))){
					flag = false;
					break;
				}
			}	
			if(flag){
				actual->addNeighbors(v);
				set.erase(iter);
				--iter;
			} 
		}
	}

	//for(Nodo * b : actual->getNeighbors()){
	//	b->resetSubTrees();
	//}
	for(Node * v : set){
		Node * temp = nullptr;
		for(Node * c : actual->getNeighbors()){
			c->setDisToNode(distance(v->getVal(), c->getVal()));
			if(temp == nullptr or temp->getDisToNode() > c->getDisToNode()){
				temp = c;
			}
		}
		temp->addSubTree(v);
	}
	for(Node * b : actual->getNeighbors()){
		_buildTree(b,b->getSubTrees());
	}
}	

template <typename kPoint>
void SATree<kPoint>::print(string file){
	string fi = file + ".dot";
	ofstream archivo(fi.c_str());
	archivo<<"graph{"<<endl;
	SetOfNodes q;
	q.push_back(root);
	while(q.size() != 0){
		Node * actual = q.front();
		q.erase(q.begin());
		string n = "Node" + to_string(actual->getId());
		string n1 = n;
		n+="[label = \"";

		for(int i = 0; i < actual->getVal().size(); i++){
			n+= to_string((int)actual->getVal()[i]);
			n+=",";
		}

		n.pop_back();
		n+="\"];";

		archivo<<n<<endl;

		for(Node * v : actual->getNeighbors()){
			q.push_back(v);
			string n2 = "Node" + to_string(v->getId());
			string dir = n1 + " -- " +  n2;
			archivo<<dir<<";"<<endl;
		}
	}
	archivo<<"}";
	archivo.close();
	string command = "dot -Tpng " + fi + " -o " + file + ".png";
	system(command.c_str());
}


#endif
