/*---------------------------------------
* Programación avanzada: Actividad 2
*
* Fecha: 13-03-2020
*
* Autor: Irving Alain Aguilar Pérez - A1703171
*
* Autor: Javier Mendez Martínez - A1703446
*
* Referencias: Ejercicios de https://github.com/Manchas2k4/advanced_programming/tree/master/processes
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
void child_process(int num, int n_actual) {
	int pid;
    if (n_actual == num)
    {
        srand( getpid() );
        sleep(1);
        for(int j = 0; j < n_actual; j++){
            printf("\t");
        }
	    printf("PPID = %i, PID = %i NIVEL = %i\n", getppid(), getpid(), n_actual);
	    exit(0);
    }
    else
    {
        for(int j = 0; j < n_actual; j++){
            printf("\t");
        }
	    printf("PPID = %i, PID = %i NIVEL = %i\n", getppid(), getpid(), n_actual);
        for (int i = 0; i <= n_actual; i++)
        {
            if ( (pid = fork()) < 0 )
            {
			    perror("fork");
			    exit(-4);
		    } 
            else if (pid == 0)
            {
                n_actual++;
			    child_process(num, n_actual);
            }
            else
            {
                wait(NULL);
            }
        }
        exit(0);
    }
}
int main(int argc, char* argv[]){
	int pid, num;
    if (argc != 2) {
		fprintf(stderr, "usage: %s number\n", argv[0]);
		return -2;
	}
    num = atoi(argv[1]);
    if (num <= 1) {
		fprintf(stderr, "%s: the parameter must be a positive integer number\n", argv[0]);
		return -3;
	}
    child_process(num, 0);
	return 0;
}