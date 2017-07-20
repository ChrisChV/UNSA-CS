#include <iostream>
#include "VS.h"
#include "../Utils/Utils.h"

using namespace cimg_library;
using namespace std;

/*
string toBinary(int num){
	string res = "";
	int temp = 0;
	for(int i = 0; i < 8; i++){
		temp = pow(2,7-i);
		if(num >= temp){
			num -= temp;
			res.push_back('1');
		}
		else res.push_back('0');
	}
	return res;
}

int sToDecimal(string num){
	int res = 0;
	for(int i = 0; i < num.size(); i++){
		if(num[i] == '1') res += pow(2,num.size() - 1 - i);
	}
	return res;
}
*/

tuple<CImg<int>,CImg<int>,CImg<int>> getChannels(string fileImg){
	CImg<int> img(fileImg.c_str());
	return make_tuple(img.get_channel(0),img.get_channel(1),img.get_channel(2));
}

void * threadSetChannels(void * arg){
	ThreadArg tArg = *((ThreadArg *) arg);
	int ini = tArg.ini;
	int end = tArg.end;
	CImg<int> * res = tArg.img;
	vector<CImg<int>> * channels = tArg.res;
	for(int i = ini; i < end; i++){
		for(int j = 0; j < res->width(); j++){
			for(int k = 0; k < 3; k++){
				(*res)(j,i,0,k) = (*channels)[k](j,i,0,0);
			}
		}
	}
}

CImg<int> setChannels(CImg<int> &R, CImg<int> &G, CImg<int> &B){
	vector<CImg<int>> channels;
	CImg<int> res(R.width(),R.height(),1,3);
	channels.push_back(R);
	channels.push_back(G);
	channels.push_back(B);
	pthread_t threads[NUM_THREADS];
	ThreadArg threadsArgs[NUM_THREADS];
	int despl = R.height() / NUM_THREADS;
	for(int i = 0; i < NUM_THREADS; i++){
		threadsArgs[i].ini = i * despl;
		if(i == NUM_THREADS - 1) threadsArgs[i].end = R.height();
		else threadsArgs[i].end = threadsArgs[i].ini + despl;
		threadsArgs[i].img = &res;
		threadsArgs[i].res = &channels;
		pthread_create(&threads[i],NULL, threadSetChannels, (void *) &threadsArgs[i]);
	}

	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i],NULL);
	}

	return res;
}



void * threadBitDescompose(void * arg){
	ThreadArg tArg = *((ThreadArg *) arg);
	int ini = tArg.ini;
	int end = tArg.end;
	CImg<int> * img = tArg.img;
	vector<CImg<int>> * res = tArg.res;
	string byte = "";
	for(int i = ini; i < end; i++){
		for(int j = 0; j < img->width(); j++){
			byte = toBinary((*img)(j,i,0,0));
			for(int k = 0; k < BITS_PER_PIXEL; k++){
				if(byte[k] == '1'){
					(*res)[k](j,i,0,0) = 1;	
				}
				//el else se cumple porque la matriz esta llena de ceros.
			}
		}
	}
}

void * threadBitConposite(void * arg){
	ThreadArg tArg = *((ThreadArg *) arg);
	int ini = tArg.ini;
	int end = tArg.end;
	vector<CImg<int>> * imgs = tArg.res;
	CImg<int> * res = tArg.img;
	string byte = "";
	string temp = "";
	for(int i = ini; i < end; i++){
		for(int j = 0; j < res->width(); j++){
			byte.clear();
			for(int k = 0; k < BITS_PER_PIXEL; k++){
				temp = to_string((*imgs)[k](j,i,0,0));
				byte.push_back(temp[0]);
			}
			(*res)(j,i,0,0) = sToDecimal(byte);
		}
	}
}

vector<CImg<int>> bitDescompose(CImg<int> &img){
	vector<CImg<int>> res;
	for(int i = 0; i < BITS_PER_PIXEL; i++){
		res.push_back(CImg<int>(img.width(),img.height(),1,1,0));
	}
	pthread_t threads[NUM_THREADS];
	ThreadArg threadsArgs[NUM_THREADS];
	int despl = img.height() / NUM_THREADS;
	for(int i = 0; i < NUM_THREADS; i++){
		threadsArgs[i].ini = i * despl;
		if(i == NUM_THREADS - 1) threadsArgs[i].end = img.height();
		else threadsArgs[i].end = threadsArgs[i].ini + despl;
		threadsArgs[i].img = &img;
		threadsArgs[i].res = &res;
		pthread_create(&threads[i],NULL, threadBitDescompose, (void *) &threadsArgs[i]);
	}

	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i],NULL);
	}

	return res;
}

CImg<int> bitComposition(vector<CImg<int>> &imgs){
	CImg<int> res(imgs[0].width(),imgs[0].height(),1,1,0);
	pthread_t threads[NUM_THREADS];
	ThreadArg threadsArgs[NUM_THREADS];
	int despl = res.height() / NUM_THREADS;
	for(int i = 0; i < NUM_THREADS; i++){
		threadsArgs[i].ini = i * despl;
		if(i == NUM_THREADS - 1) threadsArgs[i].end = res.height();
		else threadsArgs[i].end = threadsArgs[i].ini + despl;
		threadsArgs[i].img = &res;
		threadsArgs[i].res = &imgs;
		pthread_create(&threads[i],NULL, threadBitConposite, (void *) &threadsArgs[i]);
	}

	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i],NULL);
	}

	return res;	

}

tuple<CImg<int>,CImg<int>> binaryVS(CImg<int> &img){
	CImg<int> share1(img.width(),img.height(),1,1);
	share1.rand(0,1);
	CImg<int> share2 = share1 ^ img;
	return make_tuple(share1,share2);
}

CImg<int> binaryVS(CImg<int> &share1, CImg<int> &share2){
	return share1 ^ share2;
}

tuple<vector<CImg<int>>,vector<CImg<int>>> binaryVS(vector<CImg<int>> &imgs){
	vector<CImg<int>> res1;
	vector<CImg<int>> res2;
	CImg<int> share1;
	CImg<int> share2;
	for(int i = 0; i < BITS_PER_PIXEL; i++){
		tie(share1,share2) = binaryVS(imgs[i]);
		res1.push_back(share1);
		res2.push_back(share2);
	}
	return make_tuple(res1,res2);
}

vector<CImg<int>> binaryVS(vector<CImg<int>> &shares1, vector<CImg<int>> &shares2){
	vector<CImg<int>> res;
	for(int i = 0; i< BITS_PER_PIXEL; i++){
		res.push_back(binaryVS(shares1[i],shares2[i]));
	}
	return res;
}

vector<CImg<int>> VS(string fileImg){ //RR GG BB
	CImg<int> *R = new CImg<int>();
	CImg<int> *G = new CImg<int>();
	CImg<int> *B = new CImg<int>();
	tie(*R,*G,*B) = getChannels(fileImg);
	vector<CImg<int>> * descomR = new vector<CImg<int>>();
	vector<CImg<int>> * descomG = new vector<CImg<int>>();
	vector<CImg<int>> * descomB = new vector<CImg<int>>();
	*descomR = bitDescompose(*R);
	delete R;
	*descomG = bitDescompose(*G);
	delete G;
	*descomB = bitDescompose(*B);
	delete B;
	vector<CImg<int>> * shares1R = new vector<CImg<int>>();
	vector<CImg<int>> * shares2R = new vector<CImg<int>>();
	vector<CImg<int>> * shares1G = new vector<CImg<int>>();
	vector<CImg<int>> * shares2G = new vector<CImg<int>>();
	vector<CImg<int>> * shares1B = new vector<CImg<int>>();
	vector<CImg<int>> * shares2B = new vector<CImg<int>>();
	tie(*shares1R,*shares2R) = binaryVS(*descomR);
	descomR->clear();
	delete descomR;
	tie(*shares1G,*shares2G) = binaryVS(*descomG);
	descomG->clear();
	delete descomG;
	tie(*shares1B,*shares2B) = binaryVS(*descomB);
	descomB->clear();
	delete descomB;
	vector<CImg<int>> res;
	res.push_back(bitComposition(*shares1R));
	res.push_back(bitComposition(*shares2R));
	shares1R->clear();
	delete shares1R;
	shares2R->clear();
	delete shares2R;

	res.push_back(bitComposition(*shares1G));
	res.push_back(bitComposition(*shares2G));
	shares1G->clear();
	delete shares1G;
	shares2G->clear();
	delete shares2G;

	res.push_back(bitComposition(*shares1B));
	res.push_back(bitComposition(*shares2B));
	shares1B->clear();
	delete shares1B;
	shares2B->clear();
	delete shares2B;

	return res;
}


CImg<int> VS(vector<CImg<int>> &shares){
	vector<CImg<int>> * shares1R = new vector<CImg<int>>();
	vector<CImg<int>> * shares2R = new vector<CImg<int>>();
	vector<CImg<int>> * shares1G = new vector<CImg<int>>();
	vector<CImg<int>> * shares2G = new vector<CImg<int>>();
	vector<CImg<int>> * shares1B = new vector<CImg<int>>();
	vector<CImg<int>> * shares2B = new vector<CImg<int>>();
	*shares1R = bitDescompose(shares[0]);
	*shares2R = bitDescompose(shares[1]);
	*shares1G = bitDescompose(shares[2]);
	*shares2G = bitDescompose(shares[3]);
	*shares1B = bitDescompose(shares[4]);
	*shares2B = bitDescompose(shares[5]);
	vector<CImg<int>> * descomR = new vector<CImg<int>>();
	vector<CImg<int>> * descomG = new vector<CImg<int>>();
	vector<CImg<int>> * descomB = new vector<CImg<int>>();
	*descomR = binaryVS(*shares1R,*shares2R);
	shares1R->clear();
	delete shares1R;
	shares2R->clear();
	delete shares2R;
	*descomG = binaryVS(*shares1G,*shares2G);
	shares1G->clear();
	delete shares1G;
	shares2G->clear();
	delete shares2G;
	*descomB = binaryVS(*shares1B,*shares2B);
	shares1B->clear();
	delete shares1B;
	shares2B->clear();
	delete shares2B;

	CImg<int> R = bitComposition(*descomR);
	descomR->clear();
	delete descomR;
	CImg<int> G = bitComposition(*descomG);
	descomG->clear();
	delete descomG;
	CImg<int> B = bitComposition(*descomB);
	descomB->clear();
	delete descomB;

	return setChannels(R,G,B);


}


