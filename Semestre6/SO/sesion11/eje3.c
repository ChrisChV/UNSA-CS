#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SEM_PAPEL 0
#define SEM_TABACO 1
#define SEM_CERILLAS 2
#define SEM_AGENTE 3

pid_t pidHoja, pidTabaco, pidCerillas, pidAgente;

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


int generarIngrediente(int ingredienteRepetido){
	int ingrediente = rand() % 3;
	if(ingrediente == ingredienteRepetido){
		while(ingrediente == ingredienteRepetido) ingrediente = rand() % 3;
	}
	return ingrediente;
}

int ingredienteFaltante(int ingrediente1, int ingrediente2){
	if((ingrediente1 == SEM_PAPEL && ingrediente2 == SEM_TABACO) || (ingrediente2 == SEM_PAPEL && ingrediente1 == SEM_TABACO)) return SEM_CERILLAS;
	if((ingrediente1 == SEM_PAPEL && ingrediente2 == SEM_CERILLAS) || (ingrediente2 == SEM_PAPEL && ingrediente1 == SEM_CERILLAS)) return SEM_TABACO;
	if((ingrediente1 == SEM_TABACO && ingrediente2 == SEM_CERILLAS) || (ingrediente2 == SEM_TABACO && ingrediente1 == SEM_CERILLAS)) return SEM_PAPEL;
}

char * mappingSem(int sem){
	if(sem == SEM_TABACO) return "Tabaco";
	if(sem == SEM_PAPEL) return "Papel";
	if(sem == SEM_CERILLAS) return "Cerillas";
}

void fumar(int fumador){
	char * ff = mappingSem(fumador);
	printf("Fumador con %s ha fumado su cigarrillo\n",ff);
}

int main(){
	srand(time(NULL));
	key_t clave;
	int idSemaforo;
	clave = ftok("/bin/ls",34);
	idSemaforo=semget(clave,4,IPC_CREAT|0600);
	semctl(idSemaforo,SEM_AGENTE,SETVAL,0);
	semctl(idSemaforo,SEM_PAPEL,SETVAL,0);
	semctl(idSemaforo,SEM_TABACO,SETVAL,0);
	semctl(idSemaforo,SEM_CERILLAS,SETVAL,0);
	levantarSemaforo(idSemaforo,SEM_AGENTE);
	if((pidHoja = fork()) == 0){
		while(1){
			esperaSemaforo(idSemaforo,SEM_PAPEL);
			fumar(SEM_PAPEL);
			levantarSemaforo(idSemaforo,SEM_AGENTE);
		}
	}
	else{
		if((pidTabaco = fork()) == 0){
			while(1){
				esperaSemaforo(idSemaforo,SEM_TABACO);
				fumar(SEM_TABACO);
				levantarSemaforo(idSemaforo,SEM_AGENTE);
			}
		}
		else{
			if((pidCerillas = fork()) == 0){
				while(1){
					esperaSemaforo(idSemaforo,SEM_CERILLAS);
					fumar(SEM_CERILLAS);
					levantarSemaforo(idSemaforo,SEM_AGENTE);
				}
			}
			else{
				while(1){
					esperaSemaforo(idSemaforo,SEM_AGENTE);
					int ingrediente1 = generarIngrediente(-1);
					int ingrediente2 = generarIngrediente(ingrediente1);
					sleep(2);
					printf("Ingredientes generados %s y %s \n",mappingSem(ingrediente1),mappingSem(ingrediente2));
					fflush(stdout);
					int fumador = ingredienteFaltante(ingrediente1,ingrediente2);
					levantarSemaforo(idSemaforo,fumador);
				}
			}
		}
	}
}
