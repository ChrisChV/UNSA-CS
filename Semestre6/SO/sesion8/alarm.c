#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int ncortes=0;
int bucle=1;

void alarma();
void cortar();

int main() {

	signal (SIGINT, cortar);
	signal (SIGALRM, alarma);
	printf("Ejemplo de signal.\n");
	printf("Pulsa varias veces CTRL C durante 15 segundo.\n");
	alarm(15);
	while(bucle);
	signal (SIGINT,SIG_IGN);
	printf("Has intentado cortar %d veces.\n",ncortes);
	printf("Hasta luego.....\n");
	exit(0);

}

void cortar( ) {
	signal (SIGINT, SIG_IGN);
	printf("Has pulsado CTRL C \n");
	ncortes++;
	signal (SIGINT, cortar);
}

void alarma ( ) {

	signal (SIGALRM, SIG_IGN);
	bucle=0;
	printf("!Alarma! \n");

}