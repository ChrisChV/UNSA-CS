#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void alarma();
void despertar();

pid_t pidH1, pidH2 , pidH3;

int main(int argc, char *argv[]){
	/*
	pid_t pid;
	if ((pid=fork()) == 0 ){ 
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
	*/
	int status1, status2;
	if((pidH2 = fork()) ==  0){
		while(1){
			signal(SIGUSR1,despertar);
			pause();
		}		
	}
	else{
		if((pidH1 = fork()) == 0){
			if((pidH3 = fork()) == 0){
				3system("sh script.sh");
			}
			else{
				printf("Soy el hijo1 -> %d y mi padre es %d\n",getpid(),getppid());
				signal(SIGALRM,alarma);
				alarm(20);
				while(1);
			}	
			
		}
		else{
			waitpid(pidH1,&status1,0);
			waitpid(pidH2,&status2,0);
		}
	}
}

void alarma(){
	signal(SIGALRM,SIG_IGN);
	kill(pidH2,SIGUSR1);
	signal(SIGALRM,alarma);
	alarm(20);
}

void despertar(){
	printf("Soy el hijo2 -> %d\n",getpid());
}
