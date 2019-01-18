#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

long sum = 0;

struct Rango{
	int ini;
	int fin;
};

void * hilo(void * arg){
	struct Rango r = *((struct Rango*)arg);
	long sumt = 0;
	for(int i = r.ini; i <= r.fin; i++){
		sumt += i;
	}
	pthread_mutex_lock(&mutex);
	sum += sumt;
	pthread_mutex_unlock(&mutex);
}

int main(){
	int n = 1000000;
	int h = 4;
	struct Rango rangos[h];
	pthread_t hilos[h];
	int pib = n / h;
	int actual = 0;
	for(int i = 0; i < h; i++){
		rangos[i].ini = actual + 1;
		actual += pib;
		rangos[i].fin = actual;
		if(i == h-1) rangos[i].fin = n;
		pthread_create(&hilos[i],NULL,hilo,(void*)&rangos[i]);
	}
	for(int i = 0; i < h; i++){
		pthread_join(hilos[i],NULL);
	}
	printf("La suma total es %lu\n",sum);
}