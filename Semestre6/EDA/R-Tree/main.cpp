#include <iostream>
#include "RTree.h"

using namespace std;

int main(){
	/*
	RTree tree;
	tree.insert(make_tuple(3,4),make_tuple(4,2));
	tree.insert(make_tuple(3,9),make_tuple(6,8));
	tree.insert(make_tuple(5,5),make_tuple(6,4));
	tree.insert(make_tuple(7,6),make_tuple(9,5));
	tree.insert(make_tuple(8,8),make_tuple(9,7));
	tree.insert(make_tuple(5,7),make_tuple(6,6));
	tree.insert(make_tuple(10,6),make_tuple(12,1));
	tree.print("hola");
	*/

	RTree tree;
	tree.insert(make_tuple(2,4),make_tuple(2,4));
	tree.insert(make_tuple(4,2),make_tuple(4,2));
	tree.insert(make_tuple(6,6),make_tuple(6,6));
	tree.insert(make_tuple(8,6),make_tuple(8,6));
	tree.insert(make_tuple(10,10),make_tuple(10,10));
	tree.insert(make_tuple(10,2),make_tuple(10,2));
	tree.insert(make_tuple(14,6),make_tuple(14,6));
	tree.print("hola");
	float x, y, d;
    int n;
    while(true){
        //cin >> x >> y >> n;
        cin >> x >> y >> d;
        Point po = make_tuple(x, y);
        //vector<RTree::Region *> v = tree.searchNeighbors(po, n);
        vector<RTree::Region *> v = tree.searchRange(po, d);
        for(int i = 0; i < v.size(); i++)
            cout << get<0>(v[i]->limites[TOP]) << ", " << get<1>(v[i]->limites[TOP]) << endl;
    }

}