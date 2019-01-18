#include <iostream>
#include <Pila.h>
#include <stack>

using namespace std;

int main()
{
    Pila<int> my;
    for(int i = 0; i < 10; i++){
        my.push(i);
    }
    my.pop();
    my.print();
    cout<<endl<<my.top();
}
