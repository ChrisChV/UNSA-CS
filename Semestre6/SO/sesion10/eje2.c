#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


int buffer;
int *array;
int freePos = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void mostrarArray(){
	for(int i = 0; i < buffer; i++){
		printf("%d\n", array[i]);
	}
}

void * funcion(void * arg){
	int i = *((int*) arg);
	while(1){
		pthread_mutex_lock(&mutex);
		if(freePos == buffer){
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		
		array[freePos] = i;

		printf("Escribi %d en la posicion %d del buffer\n",i,freePos);
		freePos++;
		pthread_mutex_unlock(&mutex);
		sleep(i);
	}
}

int main(){
	printf("Ingrese el tamaño del buffer->");
	scanf("%d",&buffer);
	int n = 4;
	pthread_t hilos[n];
	int args[n];
	array = malloc(buffer * sizeof *array);
	for(int i = 0; i < n; i++){
		args[i] = i + 1;
		pthread_create(&hilos[i],NULL,funcion,(void *) &args[i]);
		
	}
	for(int i = 0; i < n; i++){
		pthread_join(hilos[i], NULL);
	}
	mostrarArray();
}