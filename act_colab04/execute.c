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
char cmd1[100], cmd2[100], cmd3[100];
// void readFile(){
//     FILE *file = fopen("file1.txt", "r");
//     if(file){
//         char **array = malloc(sizeof(char *) * 10);
//         char filestring[1];
//         char buffer[BUFSIZ];
//         for (size_t i = 0; fgets(buffer, sizeof buffer, file); i++)
//         {
//             array[i] = malloc(sizeof buffer);
//             strcpy(array[i], buffer);
//         }
//         // filestring[0] = array[0];
//         memcpy(filestring, array[0], 1);
//         for(int k = 0; k < 1; k++){
//             printf("%s\n", file[k]);
//         }
//     }

// }


// int readdFile( ){
//     int source;
//     char *cmd;
//     char *argv[100];
//     char s[2] = ",";
//     FILE *file = fopen("file1.txt", "r");
//     if(file){
//         char *buffer;
//         int size = lseek(source, 0, SEEK_END);
//         buffer = (unsigned int*) malloc(sizeof(unsigned int) * size);
//         lseek(source, 0, SEEK_SET);
//         read(source, buffer, size);

//         cmd = strtok(buffer, s);
//         argv[0] = cmd;
//         cmd = strtok(NULL, s);
//         int i = 1;
//         for (i = 1; cmd != NULL; i++)
//         {
//             cmd = strtok(NULL, " ");
//             argv[i] = cmd;
//         }
//         argv[i] = 0;
//         for (int i = 0; i < sizeof argv; i++)
//         {
//             printf("%s\n", argv[i]);
//         }
//     }

//}

/* Read commands from file */
int readFromFile(char* filename, char* buffer){
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
}/* Separate commas */
void separateCommas(char cmdopt[]){
    l1 = strtok(cmdopt, ",");
    l2 = strtok(NULL, ",");
}
/* Signal Handler */
void processSignal(int signalnum){
    int pid;
    switch (signalnum)
    {
    case SIGUSR1:
            if ((pid = fork()) < 0)
            {
                perror("No child process");
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
                perror("No child process");
            }
            else if (pid == 0)
            {
                execlp(l1, l2, NULL);
            }
            sleep(3);
            break;
        case SIGPWR:
            if ((pid = fork()) < 0)
            {
                perror("No child process");
            }
            else if (pid == 0)
            {
                execlp(l1, l2, NULL);
            }
            sleep(3);
            break;
        case SIGINT:
            printf("Ending...");
            _exit(0);
            break;         
    }
}
int main(int argc, char* argv[]) {
    int file1, file2, file3;
    char cmd[100];
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
    readFromFile(argv[1], cmd1);
    separateCommas(&cmd1);
    // FILE 2
    readFromFile(argv[2], cmd2);
    separateCommas(&cmd2);
    // FILE 3
    readFromFile(argv[3], cmd3);
    separateCommas(&cmd3);
    // Signals
    signal(SIGUSR1, processSignal);
    signal(SIGUSR2, processSignal);
    signal(SIGPWR, processSignal);
    signal(SIGINT, processSignal);
    printf("Waiting for signal...");
    while(1);
  return 0;
}