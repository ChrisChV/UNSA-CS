#include <iostream>
#include "Automata.h"

using namespace std;

int main()
{
    Automata<char> at("automata2");
    at.print();
    auto temp = at.construccionSubconjuntos();
    temp.generate();
    temp.print();

}
