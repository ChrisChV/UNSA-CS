#ifndef STENO_H
#define STENO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <ctime>
#include "../Genetico/Genetico.h"
#include "LSB.h"
#include "../Utils/Utils.h"


using namespace std;

/**
	#define L_BLOCK 6
	#define N_LSB 3

	#define N_PROBLACION 30
	#define ITERATIONS 20 <-----
	20 2 primeros si se llega a la igualdad total.
	10 1 si llega igualdad total, 2 no llega igualdad total.

**/


#define NUMBER_CROM 1
#define NUMBER_DIV 2
#define NUMBER_FEN 1


#define L_BLOCK 8
#define N_LSB 3

#define N_PROBLACION 30
#define PROB_MUTACION 0.3
#define ITERATIONS 20


struct PthreadArg{
	CImg<int> * share;
	string * res;
	int ini;
	int end;
};


vector<string> vSTRl;
vector<string> vSTRm;
string STRm_actual;
int SIZE_CROM = 0;

/*
void segmentation(string STRl, string STRm);
Val toDecimal(string num);
Val * funFen(Cromosoma * cromosomas);
Val funFit(Val * fenotipos);
void funMutar(Individuo & in);
vector<Individuo> funCross(Individuo madre, Individuo padre);
string hide(string msg, string fileImg);
string keyGeneration(vector<int> vKey);
*/


struct ChoseOneExeption{
	ChoseOneExeption(string er){e = er;};
	string e;
};

void segmentation(string STRl, string STRm){
	vSTRl.clear();
	vSTRm.clear();
	string temp = "";
	int count = 0;
	for(char c : STRl){
		count++;
		temp.push_back(c);
		if(count == L_BLOCK){
			vSTRl.push_back(temp);
			temp.clear();
			count = 0;
		}
	}
	if(count != 0){
		vSTRl.push_back(temp);
		temp.clear();
		count = 0;
	}
	for(char c : STRm){
		count++;
		temp.push_back(c);
		if(count == L_BLOCK){
			vSTRm.push_back(temp);
			temp.clear();
			count = 0;
		}	
	}
	if(count != 0){
		vSTRm.push_back(temp);
		temp.clear();
		count = 0;
	}
}

Val toDecimal(string num){
	Val res = 0;
	for(int i = 0; i < num.size(); i++){
		if(num[i] == '1') res += pow(2,num.size() - 1 - i);
	}
	return res;
}

Val * funFen(Cromosoma * cromosomas){
	Val * res = new Val[1];
	res[0] = cromosomas[0].binaryToDecimal();
	return res;
}

Val funFit(Val * fenotipos){
	Val res = 0;
	if(fenotipos[0] >= vSTRl.size() - 1) return -1;
	string a(vSTRl[fenotipos[0]]);
	for(int i = 0; i < L_BLOCK; i++){
		if(a[i] == STRm_actual[i]) res++;
	}
	return res;
}

void funMutar(Individuo & in){
	int bit = rand() % SIZE_CROM;
	if(in.cromosomas[0].vector[bit] == '0'){
		in.cromosomas[0].vector[bit] = '1';
	}
	else in.cromosomas[0].vector[bit] = '0';
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
//		cout<<"VECM->"<<madre.cromosomas[i]<<endl;
//		cout<<"VECP->"<<padre.cromosomas[i]<<endl;
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

	cromosomas1[0] = Cromosoma(SIZE_CROM,cortesPadre[0][0], cortesMadre[0][1]);
	res.push_back(Individuo(cromosomas1,NUMBER_CROM,NUMBER_FEN,funFen,funFit));

	
	for(int i = 0; i < NUMBER_CROM; i++){
		for(int j = 0; j < NUMBER_DIV; j++){
			free(cortesMadre[i][j]);
			free(cortesPadre[i][j]);
		}
	}

	return res;
}




void * getSTRmFromImg(void * arg){
	PthreadArg pArg = *((PthreadArg *) arg);
	CImg<int> * share = pArg.share;
	string * res = pArg.res;
	int ini = pArg.ini;
	int end = pArg.end;
	for(int i = ini; i < end; i++){
		for(int j = 0; j < share->width(); j++){
			//cout<<"IJ->"<<i<<" "<<j<<endl;
			(*res) += toBinary((*share)(j,i,0,0));
		}
	}
	cout<<"LLLLL"<<endl;
}

/*
void * getImgFromSTRm(void * arg){
	PthreadArg pArg = *((PthreadArg *) arg);
	CImg<int> * res = pArg.share;
	string * msg = pArg.res;
	int ini = pArg.ini;
	int end = pArg.end;
	for(int i = ini; i < end; i++){
		for(int j = 0; j < share.width(); j++){
					
		}
	}
}
*/

tuple<string,string> __hide(){
	vector<int> resKey;
	vector<bool> repeat;
	string STRl = "";
	string key = "";
	string temp = "";
	temp = toBinary(vSTRl.size());
	SIZE_CROM = temp.size();
	vector<int> sizeCromos = {SIZE_CROM};
	repeat = vector<bool>(vSTRl.size());
	Genetico gen;
	int choseOne = 0;
	int noChoseOneCount = 0;
	int repeatBlock = 0;
	bool flag = 0;

	for(int i = 0; i < vSTRm.size(); i++){
		STRm_actual = vSTRm[i];
		gen = Genetico(N_PROBLACION, sizeCromos, NUMBER_FEN, PROB_MUTACION, funFen, funFit, funMutar, funCross);
		gen.run(ITERATIONS);
		for(int j = 0; j < N_PROBLACION; j++){
			choseOne = gen.poblacion[j].fenotipos[0];
			if(repeat[choseOne] == 0){
				repeat[choseOne] = 1;
				vSTRl[choseOne] = vSTRm[i];
				flag = 0;
				break;
			}
			else{
				if(gen.poblacion[j].fitness == L_BLOCK){
					flag = 0;
					break;		
				}
			}
			flag = 1;
		}
		if(flag == 1){
			flag = 0;
			noChoseOneCount++;
			for(int j = 0; j < repeat.size(); j++){
				if(repeat[j] == 0){
					flag = 1;
					choseOne = j;
					repeat[choseOne] = 1;
					vSTRl[choseOne] = vSTRm[i];
					break;
				}
			}
			if(flag == 0){
				throw(ChoseOneExeption(vSTRm[i]));
			}
		}
		resKey.push_back(choseOne);
	}
	
	//for(int i = 0; i < vSTRm.size(); i++){
		//vSTRl[resKey[i]] = vSTRm[i];
	//}

	for(string s : vSTRl){
		STRl += s;
	}

	//changeImg(make_tuple(STRl,img1,img2),fileImg,N_LSB,outImg);

	for(int i : resKey){
		key += to_string(i) + " ";
	}

	key += "$ ";

	ofstream tt("keytemp1");
	tt<<key<<endl;
	tt.close();

	cout<<"\t\tNo Chose One->"<<noChoseOneCount<<endl;

	return make_tuple(key,STRl);
}


string _hide(vector<CImg<int>> shares, string fileImg){
	srand(time(NULL));
	vector<string> imgSTR;
	vector<string> resSTRls;
	vector<string> keys;
	string msg = "";
	string resKey;
	for(int i = 0; i < NUMBER_CHANNELS; i++){
		imgSTR.push_back("");
		resSTRls.push_back("");
		keys.push_back("");
	}
	tie(imgSTR[0],imgSTR[1],imgSTR[2]) = getLSBfromImg(fileImg, N_LSB);
	pthread_t threads[NUM_THREADS];
	PthreadArg threadArgs[NUM_THREADS];
	int despl = shares.front().height() / NUM_THREADS;
	vector<string> resThreads(NUMBER_CHANNELS);
	clock_t ini;
	clock_t end;
	double total = 0;
	for(int i = 0; i < NUMBER_CHANNELS; i++){
		ini = clock();
		cout<<"\tShare "<<i+1<<"..."<<endl;
		msg.clear();
		/*
		for(int j = 0; j < NUM_THREADS; j++){
			resThreads[j] = "";
			threadArgs[j].ini = j * despl;
			if(j == NUM_THREADS - 1) threadArgs[j].end = shares.front().height();
			else threadArgs[j].end = threadArgs[j].ini + despl;
			threadArgs[j].res = &resThreads[j];
			threadArgs[j].share = &shares[i];
			pthread_create(&threads[j],NULL,getSTRmFromImg,(void *) &threadArgs[j]);
		}

		for(int j = 0; j < NUM_THREADS; j++){
			cout<<"alskdlaskldaks"<<endl;
			pthread_join(threads[j], NULL);
			cout<<"holaaaaaa"<<endl;
			msg += resThreads[j];
			cout<<"holaaaaaa"<<endl;
		}
		*/
		for(int j = 0; j < shares[i].height(); j++){
			for(int k = 0; k < shares[i].width(); k++){
				msg += toBinary(shares[i](k,j,0,0));
			}
		}
		if(i == 0){
			ofstream tt("msgbin1");
			tt<<msg<<endl;
			tt.close();
		}
		segmentation(imgSTR[i],msg);
		string tempt = "";
		tie(keys[i],tempt) = __hide();
		resSTRls[i] = "";
		for(string s : vSTRl){
			resSTRls[i] += s;
		}
		//tie(keys[i],resSTRls[i]) = __hide();
		end = clock();
		total = (double) (end - ini) / CLOCKS_PER_SEC;
		cout<<"\t\tTiempo Gastado->"<<total<<endl;
		cout<<"\tShare "<<i+1<<" OK"<<endl;
		
	}

	string outImg = "out" + fileImg;

	changeImg(make_tuple(resSTRls[0],resSTRls[1],resSTRls[2]),fileImg,N_LSB,outImg);

	/*
	ofstream ttt("STRl1");
	ttt<<resSTRls[0]<<endl;
	ttt.close();
	ofstream ttt3("STRl2");
	ttt3<<imgSTR[0]<<endl;
	ttt3.close();
	tie(imgSTR[0],imgSTR[1],imgSTR[2]) = getLSBfromImg(outImg, N_LSB);
	ofstream ttt2("STRl3");
	ttt2<<outImg<<endl;
	ttt2<<imgSTR[0]<<endl;
	ttt2.close();
	*/


	for(string s : keys){
		resKey += s;
	}

	return resKey;
}

//six shares
string hide(vector<CImg<int>> shares, string fileImg1, string fileImg2, string keyFile){
	vector<CImg<int>> shares1;
	vector<CImg<int>> shares2;
	for(int i = 0; i < NUMBER_SHARES; i++){
		if(i % 2 == 0) shares1.push_back(shares[i]);
		else shares2.push_back(shares[i]);
	}
	string key = "";
	key += "out" + fileImg1 + " " + "out" + fileImg2 + " " + to_string(shares.front().height()) + " " + to_string(shares.front().width()) + " ";
	cout<<"Ocultar en stegoImg1..."<<endl;
	key += _hide(shares1,fileImg1);
	cout<<"Ocultar en stegoImg1 OK"<<endl;
	cout<<"Ocultar en stegoImg2..."<<endl;
	key += _hide(shares2,fileImg2);
	cout<<"Ocultar en stegoImg2 OK"<<endl;
	ofstream archivo(keyFile.c_str());
	archivo<<key<<endl;
	archivo.close();
	return key;
}


// La key al comienzo tine los archivos de las img huesped y el tamaño de los shares.

vector<CImg<int>> reveal(string keyFile){
	vector<CImg<int>> res(NUMBER_SHARES);
	vector<string> imgSTR;
	for(int i = 0; i < NUMBER_CHANNELS; i++){
		imgSTR.push_back("");
	}

	vector<int> key;
	string msgBin = "";
	string temp = "";
	string pixel = "";
	string fileImg[2];
	int height = 0;
	int width = 0;
	ifstream archivoKey(keyFile);
	archivoKey>>fileImg[0];
	archivoKey>>fileImg[1];
	archivoKey>>height;
	archivoKey>>width;

	for(int i = 0; i < NUMBER_STEGO_IMG; i++){
		tie(imgSTR[0], imgSTR[1], imgSTR[2]) = getLSBfromImg(fileImg[i], N_LSB);
		for(int j = 0; j < NUMBER_CHANNELS; j++){
			msgBin.clear();
			key.clear();
			while(archivoKey>>temp){
				if(temp == "$") break;
				key.push_back(stoi(temp));
			}


			


			segmentation(imgSTR[j],"");
			for(int k : key){
				msgBin += vSTRl[k];
			}


			res[j * NUMBER_STEGO_IMG + i] = CImg<int>(width,height,1,1);

			/*
			for(int k = 0; k < height; k++){
				for(int m = 0; m < width; m++){
					pixel.clear();
					for(int l = 0; l < BITS_PER_PIXEL; l++){
						pixel.push_back(msgBin[(k * width + m) * BITS_PER_PIXEL + l]);
					}
					//cout<<LSB<<endl;
					//cout<<toDecimal(LSB)<<endl;
					res[j * NUMBER_STEGO_IMG + i](m,k,0,0) = toDecimal(pixel);
				}
			}
			*/
			
			int actual = 0;
			for(int k = 0; k < height; k++){
				for(int m = 0; m < width; m++){
					pixel.clear();
					for(int l = 0; l < BITS_PER_PIXEL; l++){
						pixel.push_back(msgBin[actual]);
						actual++;
					}
					res[j * NUMBER_STEGO_IMG + i](m,k,0,0) = toDecimal(pixel);
				}
			}


		}
	}

	return res;

}



#endif
