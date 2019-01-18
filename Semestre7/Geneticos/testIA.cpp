#include <iostream>
#include "Genetico.h"

#define A 10000
#define centerX 4096
#define centerY 4096
#define oX 1500
#define oY 1500

#define NUMBER_CROM 2
#define NUMBER_DIV 2
#define NUMBER_FEN 2
#define SIZE_CROM 13

using namespace std;

Val gauss(Val x, Val y){
	return A * exp(-( pow(x-centerX,2) / (2*pow(oX,2)) + pow(y-centerY,2) / (2*pow(oY,2))));
}

Val * funFen(Cromosoma * cromosomas){
	Val * res = new Val[2];
	res[0] = cromosomas[0].binaryToDecimal();
	res[1] = cromosomas[1].binaryToDecimal();
	return res;
}

Val funFit(Val * fenotipos){
	return gauss(fenotipos[0],fenotipos[1]);
}

void funMutar(Individuo & in){
	int cromosoma = rand() % NUMBER_CROM;
	int bit = rand() % SIZE_CROM;
	if(in.cromosomas[cromosoma].vector[bit] == '0'){
		in.cromosomas[cromosoma].vector[bit] = '1';
	}
	else in.cromosomas[cromosoma].vector[bit] = '0';
}

vector<Individuo> funCross(Individuo madre, Individuo padre){
	vector<Individuo> res;
	int corte1 = rand() % (SIZE_CROM - 1) + 1;
	int corte2 = SIZE_CROM - corte1;

	char * cortesMadre[NUMBER_CROM][NUMBER_DIV];
	char * cortesPadre[NUMBER_CROM][NUMBER_DIV];

	for(int i = 0; i < NUMBER_CROM; i++){
		cortesMadre[i][0] = (char *) malloc(corte1 + 1);
		cortesMadre[i][1] = (char *) malloc(corte2 + 1);
		cortesPadre[i][0] = (char *) malloc(corte1 + 1);
		cortesPadre[i][1] = (char *) malloc(corte2 + 1);
		memcpy(cortesMadre[i][0],madre.cromosomas[i].vector,corte1);
		memcpy(cortesMadre[i][1],&(madre.cromosomas[i].vector[corte1]),corte2);
		memcpy(cortesPadre[i][0],padre.cromosomas[i].vector,corte1);
		memcpy(cortesPadre[i][1],&(padre.cromosomas[i].vector[corte1]),corte2);
		cortesMadre[i][0][corte1] = '\0';
		cortesMadre[i][1][corte2] = '\0';
		cortesPadre[i][0][corte1] = '\0';
		cortesPadre[i][1][corte2] = '\0';
	}

	Cromosoma * cromosomas1 = new Cromosoma[NUMBER_CROM];

	Cromosoma * cromosomas2 = new Cromosoma[NUMBER_CROM];

	cromosomas1[0] = Cromosoma(SIZE_CROM,cortesMadre[0][0], cortesPadre[0][1]);

	cromosomas1[1] = Cromosoma(SIZE_CROM,cortesMadre[1][0], cortesPadre[1][1]);

	res.push_back(Individuo(cromosomas1,NUMBER_CROM,NUMBER_FEN,funFen,funFit));
	cromosomas2[0] = Cromosoma(SIZE_CROM,cortesPadre[0][0], cortesMadre[0][1]);
	cromosomas2[1] = Cromosoma(SIZE_CROM,cortesPadre[1][0], cortesMadre[1][1]);
	res.push_back(Individuo(cromosomas2,NUMBER_CROM,NUMBER_FEN,funFen,funFit));

	
	for(int i = 0; i < NUMBER_CROM; i++){
		for(int j = 0; j < NUMBER_DIV; j++){
			free(cortesMadre[i][j]);
			free(cortesPadre[i][j]);
		}
	}
	
	return res;
}

int main(){
	srand(time(NULL));
	vector<int> sizeCromos = {SIZE_CROM, SIZE_CROM};
	Genetico gen(30,sizeCromos,NUMBER_FEN,0.3,funFen,funFit,funMutar,funCross);
	gen.run(20);



}