#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>     
#include <time.h> 

int tamBuffer = 10;
int count = 0;
int nextLee = 0;
int nextEscribe = 0;
int *buffer;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t lleno = PTHREAD_COND_INITIALIZER;
pthread_cond_t vacio = PTHREAD_COND_INITIALIZER;

void lee(){
	printf("Consumidor ha consumido el valor %d\n",buffer[nextLee]);
	sleep(1);
	pthread_mutex_lock(&mutex2);
	count--;
	pthread_mutex_unlock(&mutex2);
	nextLee++;
	if(nextLee  == tamBuffer) nextLee = 0;
}

void escribe(){
	buffer[nextEscribe] = rand() % 100;	
	printf("Productor ha producido el valor %d\n",buffer[nextEscribe]);
	pthread_mutex_lock(&mutex2);
	count++;
	pthread_mutex_unlock(&mutex2);
	nextEscribe++;
	if(nextEscribe == tamBuffer) nextEscribe = 0;
}

void * consumidor(void * arg){
	while(1){
//		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex2);
		if(count == 0){
			pthread_cond_signal(&vacio);
			pthread_cond_wait(&lleno, &mutex);
		}
		pthread_mutex_unlock(&mutex2);
		lee();
		pthread_cond_signal(&vacio);
//		pthread_mutex_unlock(&mutex);
	}
}

void * productor(void * arg){
	while(1){
//		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex2);
		if(count == tamBuffer){
			pthread_cond_signal(&lleno);
			pthread_cond_wait(&vacio,&mutex);
		}
		pthread_mutex_unlock(&mutex2);
		escribe();
		pthread_cond_signal(&lleno);
//		pthread_mutex_unlock(&mutex);
	}
}

int main(){
	srand(time(NULL));
	buffer = malloc(tamBuffer * sizeof *buffer);
	pthread_t con, pro;

	//pthread_create(&con,NULL,consumidor,NULL);
	pthread_create(&pro,NULL,productor,NULL);

	//pthread_join(con,NULL);

	
	while(1){
	//		pthread_mutex_lock(&mutex);
			if(count == 0){
				pthread_cond_signal(&vacio);
				pthread_cond_wait(&lleno, &mutex);
			}
			lee();
			pthread_cond_signal(&vacio);
	//		pthread_mutex_unlock(&mutex);
	}
	//pthread_join(pro,NULL);
}