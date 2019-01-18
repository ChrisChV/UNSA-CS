#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pid_t pid1,pid2;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *_buf;
};

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


int main() {
	int *bucle = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*bucle = 20;
	int status1, status2;
	key_t clave;
	int idSemaforo;
	clave = ftok("/bin/ls",32);
	idSemaforo=semget(clave,3,IPC_CREAT|0600);
	semctl(idSemaforo,0,SETVAL,0);
	semctl(idSemaforo,1,SETVAL,0);
	semctl(idSemaforo,2,SETVAL,0);
	levantarSemaforo(idSemaforo,0);
	if((pid1 = fork()) == 0){
		while(1){
			esperaSemaforo(idSemaforo,2);
			if(*bucle == 0){
				levantarSemaforo(idSemaforo,0);
				levantarSemaforo(idSemaforo,1);
				break;
			}
			for (int i = 0; i < *bucle; i++) {
				printf("+");
				fflush(stdout);
			}	
			printf("\n");
			fflush(stdout);
			*bucle = *bucle - 1;
			levantarSemaforo(idSemaforo,0);
		}
	}
	else{
		if((pid2 = fork()) == 0){
			while(1){
				esperaSemaforo(idSemaforo,1);
				if(*bucle == 0){
					levantarSemaforo(idSemaforo,0);
					levantarSemaforo(idSemaforo,2);
					break;
				}
				for (int i = 0; i < *bucle; i++) {
					printf("o");
					fflush(stdout);
				}
				printf("\n");
				fflush(stdout);
				*bucle = *bucle - 1;
				levantarSemaforo(idSemaforo,2);
			}
		}
		else{
			while(1){
				esperaSemaforo(idSemaforo,0);
				if(*bucle == 0){
					levantarSemaforo(idSemaforo,1);
					levantarSemaforo(idSemaforo,2);
					break;
				}
				for(int i = 0; i < *bucle; i++){
					printf("x");
					fflush(stdout);
				}
				printf("\n");
				fflush(stdout);
				*bucle = *bucle - 1;
				levantarSemaforo(idSemaforo,1);
			}
		}
	}
	exit(0);
}