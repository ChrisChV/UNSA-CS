#include <iostream>
#include "Automata.h"

using namespace std;

int main()
{
    Automata<char> at("automata1");
    auto temp = at.construccionSubconjuntos();
    temp.metadata();
}
