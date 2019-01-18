#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

int sum = 0;

void * hilo(void * arg){
	int id = *((int*)arg);
	int sumt = 0;
	for(int i = 0; i < 10; i++){
		int r = rand() % 100 + 1;
		sumt += r;
		printf("Soy el hilo %d, numero generado -> %d\n",id,r);	
		sleep(rand() % 3 + 1);
	}
	printf("Soy el hilo %d, la suma obtenida es %d\n",id,sumt);
	sum += sumt;
}

int main(){
	int n = 3;
	srand(time(NULL));
	pthread_t hilos[n];
	int id[3];
	for(int i = 0; i < n; i++){
		id[i] = i;
		pthread_create(&hilos[i],NULL,hilo,(void*)&id[i]);
	}
	for(int i = 0; i < n; i++){
		pthread_join(hilos[i],NULL);
	}
	printf("Soy el padre, la suma total es %d\n",sum);
}