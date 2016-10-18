#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void tratasenial (int);

int main() {

	pid_t idProceso;
	idProceso = fork();
	if (idProceso==-1) {
		perror("No se puede lanzar proceso");
		exit(-1);
	}

	if (idProceso == 0) {
		signal (SIGUSR1, tratasenial);
		while (1)
		pause();
	}

	if (idProceso > 0) {
		while (1) {
			sleep(1);	|
			kill (idProceso, SIGUSR1);
		}

	}

}

void tratasenial (int nsenial) {
	printf ("Recibida la señal del Padre\n");
}