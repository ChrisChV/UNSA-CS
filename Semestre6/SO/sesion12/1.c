#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define SEM_RUEDAS 0
#define SEM_CUADRO 1
#define SEM_MANILLAR 2

key_t clave;
int idSemaforo;
sem_t semaforos[3];
int N;
int M;


void esperaSemaforo(int idSemaforo, int indice){
	struct sembuf operacion;
	operacion.sem_num = indice;
	operacion.sem_op = -1;
	operacion.sem_flg = 0;
	semop(idSemaforo,&operacion,1);
}

void levantarSemaforo(int idSemaforo, int indice){
	struct sembuf operacion;
	operacion.sem_num = indice;
	operacion.sem_op = 1;
	operacion.sem_flg = 0;
	semop(idSemaforo,&operacion,1);
}


void * OP1(void* arg){
	while(1){
		int val;
		sem_getvalue(&semaforos[SEM_RUEDAS],&val);
		if(val == M){
			esperaSemaforo(idSemaforo,SEM_RUEDAS);
		}
		else{
			sem_post(&semaforos[SEM_RUEDAS]);
			printf("Operario1 hizo una rueda\n");
			sleep(1);
		}
	}
}

void * OP2(void * arg){
	while(1){
		int val;
		sem_getvalue(&semaforos[SEM_CUADRO],&val);
		if(val == M){
			esperaSemaforo(idSemaforo,SEM_CUADRO);
		}
		else{
			sem_post(&semaforos[SEM_CUADRO]);
			printf("Operario2 hizo un cuadro\n");
			sleep(1);
		}
	}
}

void * OP3(void *arg){
	while(1){
		int val;
		sem_getvalue(&semaforos[SEM_MANILLAR],&val);
		if(val == M){
			esperaSemaforo(idSemaforo,SEM_MANILLAR);
		}
		else{
			sem_post(&semaforos[SEM_MANILLAR]);
			printf("Operario3 hizo un manillar\n");
			sleep(1);
		}
	}
}

int main(int argc, char * argv[]){

	if(argc != 3){
		printf("Faltan argumentos\n");
		return 0 ;	
	} 

	sscanf(argv[1],"%d",&N);
	sscanf(argv[2],"%d",&M);


	pthread_t op1,op2,op3;
	
	clave = ftok("/bin/ls",40);
	idSemaforo=semget(clave,6,IPC_CREAT|0600);
	semctl(idSemaforo,SEM_RUEDAS,SETVAL,0);
	semctl(idSemaforo,SEM_CUADRO,SETVAL,0);
	semctl(idSemaforo,SEM_MANILLAR,SETVAL,0);

	
	sem_init(&semaforos[SEM_RUEDAS],1,0);
	sem_init(&semaforos[SEM_CUADRO],1,0);
	sem_init(&semaforos[SEM_MANILLAR],1,0);


	pthread_create(&op1,NULL,OP1,NULL);
	pthread_create(&op2,NULL,OP2,NULL);
	pthread_create(&op3,NULL,OP3,NULL);

	while(1){
		sem_wait(&semaforos[SEM_RUEDAS]);
		levantarSemaforo(idSemaforo,SEM_RUEDAS);
		sem_wait(&semaforos[SEM_RUEDAS]);
		levantarSemaforo(idSemaforo,SEM_RUEDAS);
		sem_wait(&semaforos[SEM_MANILLAR]);
		levantarSemaforo(idSemaforo,SEM_MANILLAR);
		sem_wait(&semaforos[SEM_CUADRO]);
		levantarSemaforo(idSemaforo,SEM_CUADRO);
		printf("El OP4 hizo una bicileta\n");
		sleep(1);
	}
}


