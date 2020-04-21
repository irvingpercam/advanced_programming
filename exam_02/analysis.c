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
void handler(int signal, int iteration, char *input, char *output){
  switch(signal){
      case SIGUSR1:
        printf("ONE: Received SIGUSR1 from ANALYSIS. Starting...");

        char src[100], itr[2], format[10];
        int size;

        strcpy(src, input);
        size = sprintf(itr,"%d",iteration);
        strcpy(format, ".dat");

        strcat(src, itr);
        strcat(src, format);
        printf("Final string : %s\n",src);

        int file;
        if((file = open(src, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
            printf("Can't create file");
            return -3;
        }

        printf("File: %s --> ",src);
        int num[105];
        int total = 100;
        for(int i = 0; i < total; i++){
            num[i] = (rand() % 100) + 1;
            putw(num[i], file);
            printf("%i ",num[i]);
        }

        close(file);
        break;
        case SIGUSR2: 
          printf("TWO: I received SIGUSR2 from ANALYSIS. I will start to work");
            char src[100], itr[2], format[10];
            int size;

            strcpy(src, output);
            size = sprintf(itr,"%d",iteration);
            strcpy(format, ".dat");

            strcat(src, itr);
            strcat(src, format);
            printf("Final string : %s\n",src);
            int num[105];
            int numActual;
            int sum = 0;
            for(int i = 0; i<100; i++){
                numActual = read(file1, &nums[i], 105);
                sum = sum + numActual;
            }
            float mean = sum/100;

            float varSum = 0;
            for(int i = 0; i<100; i++){
                varSum = varSum + pow(abs(num[i]-mean),2);
            }
            float variance = varSum/(99);

            int file2;
            if((file2 = open(src, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
                printf("Unable to create file");
                return -5;
            }

            putw(mean, file2);
            putw(variance, file2);

                    close(file2);
                    break;

}
int main(int argc, char const *argv[])
{
    int pid, num;
    if (argc != 4) {
		fprintf(stderr, "usage: %s timeout prefix_input prefix_output\n", argv[0]);
		return -2;
	}
    num = atoi(argv[1]);
    if (num < 1) {
		fprintf(stderr, "%s: the timeout must be a integer number greater than 0\n", argv[0]);
		return -3;
	}
  srand(SEED);
  signal(SIGUSR1, handler1);
  signal(SIGUSR2, handler2);
    // if ((pid = vfork()) < 0 ) {
    //   perror("fork");
    //   exit(-4);
    // } else if (pid == 0) {
    //   printf("I am ANALYSIS and I create to ONE");
    // } else {
    //   wait(NULL);
    // }
}
