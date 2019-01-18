#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int n;
sem_t clientes;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * barbero(void * arg){
	while(1){
		sem_wait(&clientes);
		printf("El barbero esta AFEITANDO a un cliente\n");
		sleep(1);
	}
}

void * cliente(void * arg){
	printf("Cliente ENTRA a la barberia\n");
	int val;
	pthread_mutex_lock(&mutex);
	sem_getvalue(&clientes,&val);
	if(val == n){
		pthread_mutex_unlock(&mutex);
		printf("Cliente SE VA de la barberia\n");
	 	return NULL;
	}
	pthread_mutex_unlock(&mutex);
	sem_post(&clientes);
}

int main(int argc, char * argv[]){
	if(argc != 2){
		printf("Faltan el numero de sillas\n");
		return 0;
	}
	sscanf(argv[1],"%d",&n);
	int numClientes = 1000;
	pthread_t pBarbero;
	pthread_t pClientes[numClientes];
	sem_init(&clientes,1,0);
	pthread_create(&pBarbero,NULL,barbero,NULL);
	for(int i = 0; i < numClientes; i++){
		pthread_create(&pClientes[i],NULL,cliente,NULL);
		sleep(1); // Comentar el sleep para ver como los clientes se van de la barberia.
	}
	pthread_join(pBarbero,NULL);
}