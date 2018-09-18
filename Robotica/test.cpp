#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class Padre{
    public:
    
    class Node{
        public:
        Node(int val){
            this->val = val;
        }
        int val;
        
    };
    Padre(){
        compare = this->test;
    }
    vector<Node *> vec;
    function<bool(Node *, Node *)> compare;
    void tt(int val){
        vec.push_back(new Node(val));
        push_heap(vec.begin(), vec.end(), compare);
    }
    int tt2(){
        pop_heap(vec.begin(), vec.end(), compare);
        int res = vec.back()->val;
        vec.pop_back();
        return res;
    }
    void print(){
        for(Node * n : vec){
            cout<<n->val<<" ";
        }
    }

    bool test(Padre::Node * a, Padre::Node * b){
        return a->val > b->val;
    }
};



int main(int argc, char const *argv[]){
    Padre pp;
    pp.tt(2);
    pp.tt(1);
    pp.tt(-2);
    pp.tt(3);
    pp.tt(6);
    pp.tt(-1);
    pp.print();
    cout<<endl;
    cout<<pp.tt2()<<endl;
    cout<<pp.tt2()<<endl;
    cout<<pp.tt2()<<endl;
    cout<<pp.tt2()<<endl;
    cout<<pp.tt2()<<endl;
}
