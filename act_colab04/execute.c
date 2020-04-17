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
jmp_buf env1, env2, env3;
typedef unsigned long ulong;
void handler(int sig) {
	if (sig == SIGUSR1) {
    longjmp(env1, 1);
  } else if (sig == SIGUSR2) {
    longjmp(env2, 1);
  } else if (sig == SIGHUP) {
    longjmp(env3, 1);
  }
}

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
int main(int argc, char* argv[]) {
    // int file1, file2, file3;
    // signal(SIGUSR1, handler);
    // signal(SIGUSR2, handler);
    // signal(SIGHUP, handler);

    // if (argc != 4) {
    //     fprintf(stderr, "usage: %s file1 file2 file3\n", argv[0]);
    //     return -2;
    // }
    // /* Check if the file exists */
  	// if((file1 = open(argv[1], O_RDONLY)) < 0)
    // {
    //     fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[1]);
    //   	return -3;
    // }
    // if ((file2 = open(argv[2], O_RDONLY)) < 0) {
    //     fprintf(stderr, "%s: the file %s does not exist\n", argv[0],argv[2]);
	//     return -4;
    // }
    // if ((file3 = open(argv[3], O_RDONLY)) < 0) {
    //     fprintf(stderr, "%s: the file %s does not exist\n", argv[0],argv[3]);
    //     return -5;
    // }
    // ulong f_size;
    // f_size = lseek(file1, 0, SEEK_END);
    // if (f_size < 1) {
    //     fprintf(stderr, "%s: the file %s is empty\n", argv[0],argv[1]);
    //     return -6; 
    // }
    // f_size = lseek(file2, 0, SEEK_END);
    // if (f_size < 1) {
    //     fprintf(stderr, "%s: the file %s is empty\n", argv[0],argv[2]);
    //     return -7; 
    // }
    // f_size = lseek(file3, 0, SEEK_END);
    // if (f_size < 1) {
    //     fprintf(stderr, "%s: the file %s is empty\n", argv[0],argv[3]);
    //     return -8; 
    // }
    quitarcomas();
  return 0;
}