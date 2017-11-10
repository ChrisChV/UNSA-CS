#include <iostream>
#include "compilador.h"
#include "error.h"
#include "utils.h"


using namespace std;

int main(int argc, char ** argv)
{
	try{
		string er = leerER();
		CompiladorErToAuto comp;
		comp.run(er);
	}
	catch(Error e){
		manejarError(e);
	}
	
}
