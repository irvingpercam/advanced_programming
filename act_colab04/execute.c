/*---------------------------------------
* Programación avanzada: Actividad 4
*
* Fecha: 17-02-2020
*
* Autor: Irving Alain Aguilar Pérez - A1703171
*
* Autor: Javier Mendez Martínez - A1703446
*
* Referencias: Ejercicios de https://github.com/Manchas2k4/advanced_programming/tree/master/signals
*----------------------------------------*/
/* [START] C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
typedef unsigned long ulong;

char *l1, *l2, *l3, *l4, *l5, *l6;
char cmd1[20], cmd2[20], cmd3[20];

/* Read commands from file */
int readFile(char* filename, char* buffer){
    FILE* file = fopen(filename, "r");
    fseek(file, 0L, SEEK_END);
    long ptr = ftell(file);
    rewind(file);
    if (buffer != NULL)
    {
        fread(buffer, ptr, 1, file);
        fclose(file);
        return 1;
    }
}
/* Signal Handler */
void signalHandler(int signalnum){
    int pid;
    switch (signalnum)
    {
        case SIGUSR1:
            if ((pid = vfork()) < 0)
            {
                perror("Failed to create child");
            }
            else if (pid == 0)
            {
                execlp(l1, l2, NULL);
                _exit(1);
            }
        break;
        case SIGUSR2:
            if ((pid = vfork()) < 0)
            {
                perror("Failed to create child");
            }
            else if (pid == 0)
            {
                execlp(l3, l4, NULL);
                _exit(1);
            }
            break;
        case SIGPWR:
            if ((pid = vfork()) < 0)
            {
                perror("Failed to create child");
            }
            else if (pid == 0)
            {
                execlp(l5, l6, NULL);
                _exit(1);
            }
            break;
        case SIGINT:
            puts("Ending...");
            _exit(0);
            break;         
    }
}
int main(int argc, char* argv[]) {
    int file1, file2, file3;
    if (argc != 4) {
        fprintf(stderr, "usage: %s file1 file2 file3\n", argv[0]);
        return -2;
    }
    /* Check if the file exists */
  	if((file1 = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[1]);
      	return -3;
    }
    if ((file2 = open(argv[2], O_RDONLY)) < 0) {
        fprintf(stderr, "%s: the file %s does not exist\n", argv[0],argv[2]);
	    return -4;
    }
    if ((file3 = open(argv[3], O_RDONLY)) < 0) {
        fprintf(stderr, "%s: the file %s does not exist\n", argv[0],argv[3]);
        return -5;
    }
    ulong f_size;
    f_size = lseek(file1, 0, SEEK_END);
    if (f_size < 1) {
        fprintf(stderr, "%s: the file %s is empty\n", argv[0],argv[1]);
        return -6; 
    }
    f_size = lseek(file2, 0, SEEK_END);
    if (f_size < 1) {
        fprintf(stderr, "%s: the file %s is empty\n", argv[0],argv[2]);
        return -7; 
    }
    f_size = lseek(file3, 0, SEEK_END);
    if (f_size < 1) {
        fprintf(stderr, "%s: the file %s is empty\n", argv[0],argv[3]);
        return -8; 
    }
    /* Get the commands from files */
    // FILE 1
    FILE* f1 = fopen(argv[1], "r");
    readFile(argv[1], cmd1);
    l1 = strtok(cmd1, ",");
    l2 = strtok(NULL, ",");
    // FILE 2
    FILE* f2 = fopen(argv[2], "r");
    readFile(argv[2], cmd2);
    l3 = strtok(cmd2, ",");
    l4 = strtok(NULL, ",");
    // FILE 3
    FILE* f3 = fopen(argv[3], "r");
    readFile(argv[3], cmd3);
    l5 = strtok(cmd3, ",");
    l6 = strtok(NULL, ",");
    // Signals
    puts("Waiting for a signal...");
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGPWR, signalHandler);
    signal(SIGINT, signalHandler);
    while(1) {
        pause();
    }
  return 0;
}