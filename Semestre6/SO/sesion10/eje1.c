#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int bucle = 20;

pthread_mutex_t mutexO = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexX = PTHREAD_MUTEX_INITIALIZER;

void *funcion01(void *arg) {
	while(1){
		pthread_mutex_lock(&mutexMas);
		if(bucle == 0){
			pthread_mutex_unlock(&mutexX);
			pthread_mutex_unlock(&mutexO);
			break;
		}
		for (int i = 0; i < bucle; i++) {
			printf("+");
			fflush(stdout);
			//sleep(1);
		}	
		printf("\n");
		fflush(stdout);
		bucle--;
		pthread_mutex_unlock(&mutexX);
	}
	
	return NULL;
}

void *funcion02(void *arg) {
	while(1){
		pthread_mutex_lock(&mutexO);
		if(bucle == 0){
			pthread_mutex_unlock(&mutexX);
			pthread_mutex_unlock(&mutexMas);
			break;
		}
		for (int i = 0; i < bucle; i++) {
			printf("o");
			fflush(stdout);
			//sleep(1);
		}
		printf("\n");
		fflush(stdout);
		bucle--;
		pthread_mutex_unlock(&mutexMas);
	}
	
	return NULL;
}

int main() {
	pthread_t mihilo01, mihilo02;
	pthread_mutex_lock(&mutexO);
	pthread_mutex_lock(&mutexMas);
	if (pthread_create(&mihilo01,NULL,funcion01,NULL)) {
		printf("Error creando el hilo.");
		abort();
	}
	if (pthread_create(&mihilo02,NULL,funcion02,NULL)) {
		printf("Error creando el hilo.");
		abort();
	}

	while(1){
		pthread_mutex_lock(&mutexX);
		if(bucle == 0){
			pthread_mutex_unlock(&mutexO);
			pthread_mutex_unlock(&mutexMas);
			break;
		}
		for(int i = 0; i < bucle; i++){
			printf("x");
			fflush(stdout);
			//sleep(1);
		}
		printf("\n");
		fflush(stdout);
		bucle--;
		pthread_mutex_unlock(&mutexO);
	}

	exit(0);
}