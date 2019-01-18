#include <pthread.h>
#include <stdio.h>

#define NUMT 10

void * fun(void* arg){
	int num_tread = *((int*) arg);
	printf("Soy el hilo num %d \n",num_tread);
}

int main(int argc, char** argv){
	pthread_t hilos[NUMT];
	int num_hilos[NUMT];
	printf("Creando Hilos");
	for(int i = 0; i < NUMT; i++){
		num_hilos[i] = i;
		pthread_create(&hilos[i],NULL,fun,&num_hilos[i]);
	}
	for(int i = 0; i < NUMT; i++){
		pthread_join(hilos[i],NULL);
	}
	printf("FIN");
}