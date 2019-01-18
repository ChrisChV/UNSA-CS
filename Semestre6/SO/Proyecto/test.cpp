#include <iostream>
#include "LectorPatron.h"

using namespace std;

int main(int argc, char * argv[]){
	string file(argv[1]);
	MappingObject o = mappingFile(file);
	printMapColas(get<0>(o));
	printMapSteps(get<1>(o));
}