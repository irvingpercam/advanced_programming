/*---------------------------------------
* Programación avanzada: Examen 2
*
* Fecha: 21-04-2020
*
* Autor: Irving Alain Aguilar Pérez - A01703171
*
*
* Referencias: Ejercicios de https://github.com/Manchas2k4/advanced_programming/tree/master/
*----------------------------------------*/
/* [START] C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h> 
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#define SEED 12345
void createProcess(int pids[2]){
  int pid;
  for (int i = 0; i < 2; i++)
  {
    if ((pid = vfork()) < 0)
    {
      perror("fork");
    }
    else if (pid == 0)
    {
      pids[i] = pid;
    }
  }
  for (int i = 0; i < 2; i++)
  {
    printf("The process %d was created.", pids[i]);
  }
  
  
}
int main(int argc, char const *argv[])
{
    int pids[2], num;
    if (argc != 4) {
		fprintf(stderr, "usage: %s timeout prefix_input prefix_output\n", argv[0]);
		return -2;
	}
    num = atoi(argv[1]);
    if (num < 1) {
		fprintf(stderr, "%s: the timeout must be a integer number greater than 0\n", argv[0]);
		return -3;
	}
  /* CREATE PROCESSES */
  
return 0;
}
