#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	pid_t pid;
	if ( (pid=fork()) == 0 ){ 
		printf("1->Soy el hijo (%d, hijo de %d)\n", getpid(), getppid());
		sleep(10);
		printf("2->Soy el hijo (%d, hijo de %d)\n", getpid(), getppid());
	}
	else{ 
		printf("1->Soy el padre (%d, mi hijo es %d)\n", getpid(), pid);
		sleep(30);
		printf("2->Soy el padre (%d, mi hijo es %d)\n", getpid(), pid);
	}
	return 0;

}