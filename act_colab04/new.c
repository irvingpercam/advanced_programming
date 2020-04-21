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
typedef unsigned long ulong;
char *l1, *l2, *l3, *l4, *l5, *l6;
char cmd1[10000], cmd2[10000], cmd3[10000];
/* Read commands from file */
int readFromFile(char* filename, char* buffer){
    FILE* file = fopen(filename, "r");
	if (file != NULL){
		fseek(file, 0L, SEEK_END);
		long pos = ftell(file);
		rewind(file);
		if(buffer != NULL) {
			fread(buffer, pos, 1, file);
			fclose(file); 
            file = NULL;
		}
		if(file != NULL) {
            fclose(file);
        }
		return 1;
	}
	return 0;
    }
/* Separate commas */
void separateCommas(char cmdopt[]){
    l1 = strtok(cmdopt, ",");
    l2 = strtok(NULL, ",");
}
/* Signal Handler */
void processSignal(int signal){
    int pid;
    switch (signal)
    {
    case SIGUSR1:
            if ((pid = fork()) < 0)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                execlp(l1, l2, NULL);
            }
            sleep(3);
        break;
        case SIGUSR2:
            if ((pid = fork()) < 0)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                execlp(l3, l4, NULL);
            }
            sleep(3);
            break;
        case SIGPWR:
            if ((pid = fork()) < 0)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                execlp(l5, l6, NULL);
            }
            sleep(3);
            break;
        case SIGINT:
            puts("Ending...");
            _exit(0);         
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
    // FILE 1
    readFromFile(argv[1], cmd1);
    l1 = strtok(cmd1, ",");
    l2 = strtok(NULL, ",");
    if ((file2 = open(argv[2], O_RDONLY)) < 0) {
        fprintf(stderr, "%s: the file %s does not exist\n", argv[0],argv[2]);
	    return -4;
    }
    // FILE 2
    readFromFile(argv[2], cmd2);
    l1 = strtok(cmd2, ",");
    l2 = strtok(NULL, ",");
    if ((file3 = open(argv[3], O_RDONLY)) < 0) {
        fprintf(stderr, "%s: the file %s does not exist\n", argv[0],argv[3]);
        return -5;
    }
    // FILE 3
    readFromFile(argv[3], cmd3);
    l1 = strtok(cmd3, ",");
    l2 = strtok(NULL, ",");
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

    // Signals
    signal(SIGUSR1, processSignal);
    signal(SIGUSR2, processSignal);
    signal(SIGPWR, processSignal);
    signal(SIGINT, processSignal);

    puts("Waiting for a signal...");
    while(1);
    return 0;
}