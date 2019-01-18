#include <iostream>
//#include "Kohonen2d.h"
#include "Kohonen.h"

int main(){
	NeuroMap nm(20,4,10,0.5,3000,"irisN.dat","testIrisN.dat");
	nm.run();
	nm.save("res20AA","res20BB");
	
	//NeuroMap nm("res20AAA","res20BBB","TotalLearningN.dat","TotalTestN.dat");


	//nm.printTrainingImg(vec);
	//nm.runTests(1);	
	//nm.printClases();
	//nm.printTraining();
	//nm.printImg();
	//nm.print();
	/*
	Val * input = (Val *) malloc(sizeof(Val) * 3);
	input[0] = 7;
	input[1] = 8;
	input[2] = 9;
	Winner win = nm.getWin(input);
	*/
	//nm.printWin(win);

	nm.runTests();

	//nm.printTraining();

	//int colorTest = 0;
	//int winnerColor = 0;

	//nm.test(winnerColor,colorTest);
	//cout<<colorTest<<" "<<winnerColor<<endl;


}