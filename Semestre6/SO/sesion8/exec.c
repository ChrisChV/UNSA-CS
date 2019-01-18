#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	printf("MENSAJITOOO\n");
	char *args[] = { "/bin/ls", NULL };
	execv("/bin/ls", args);
	printf("Se ha producido un error al ejecutar execv.\n");
	return 0;

}