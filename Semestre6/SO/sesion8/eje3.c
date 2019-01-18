#include <stdio.h>

#include <sys/types.h>

#include <unistd.h>

int main(void) {

int c;

int p=0;

pid_t pid;

for (c = 0; c < 3 ; c++ ){

pid = fork();

}

p++;

printf("Proceso %d\n", p);

return 0;

}