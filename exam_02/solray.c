/*---------------------------------------
* Programación avanzada: Examen 2
* Fecha: 21-04-2020
* Autor: Raymundo Romero Arenas - A00570654
*----------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#define SEED 12345

typedef unsigned int uints;
typedef unsigned long ulong;

const char *input;
const char *output;
int iteration = 1;
int pids[2];

//-------------- REFACTOR -----------------

// Ejecutar el comando según la señal
void handler(int signal){
//void handler(int signal, int iteration, char *input, char *output){
    char inputFile[100], outputFile[100], itr[2], format[10];
    int size;

    strcpy(inputFile, input);
    size = sprintf(itr,"%d",iteration);

    strcpy(format, ".dat");
    strcat(inputFile, itr);
    strcat(inputFile, format);
    printf("Input filename: %s\n",inputFile);

    strcpy(outputFile, output);
    strcat(outputFile, itr);
    strcat(outputFile, format);
    printf("Output filename: %s\n",outputFile);

    switch(signal){
        case SIGUSR1:
                    printf("ONE: I received SIGUSR1 from ANALYSIS. I will start to work");
                    
                    int file;
                    if((file = open(inputFile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
                        printf("Unable to create file");
                        exit(-3);
                    }

                    uints *buffer1;
                    ulong fileSize1 = lseek(file, 0, SEEK_END);
                    buffer1 = (uints*) malloc(sizeof(uints) * fileSize1);

                    printf("File: %s --> ",inputFile);
                    int num[105];
                    int total = 100;
                    for(int i = 0; i < total; i++){
                        num[i] = (rand() % 100) + 1;
                        write(num[i], buffer1, file);
                        printf("%i ",num[i]);
                    }

                    close(file);
                    break;
        case SIGUSR2: 
                    printf("TWO: I received SIGUSR2 from ANALYSIS. I will start to work");

                    int file1;
                    if((file1 = open(inputFile, O_RDONLY)) < 0){
                        printf("Unable to open file");
                        exit(-4);
                    }
                    
                    int nums[105];
                    int numActual;
                    int sum = 0;
                    for(int i = 0; i<100; i++){
                        numActual = read(file1, &nums[i], 105);
                        sum = sum + numActual;
                    }
                    float mean = sum/100;

                    float varSum = 0;
                    for(int i = 0; i<100; i++){
                        varSum = varSum + pow(abs(nums[i]-mean),2);
                    }
                    float variance = varSum/(99);

                    int file2;
                    if((file2 = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
                        printf("Unable to create file");
                        exit(-5);
                    }

                    uints *buffer2;
                    ulong fileSize2 = lseek(file2, 0, SEEK_END);
                    buffer2 = (uints*) malloc(sizeof(uints) * fileSize2);

                    write(mean, buffer2, file2);
                    write(variance, buffer2, file2);

                    close(file2);
                    break;
    }
}

//------------ EN DESARROLLO --------------------------------------

void ONE(){
    printf("ONE: I received SIGUSR1 from ANALYSIS. I will start to work");

    char inputFile[100], itr[2];
    int size = sprintf(itr,"%i",iteration);

    strcpy(inputFile, input);
    strcat(inputFile, itr);
    strcat(inputFile, ".dat");

    printf("Input filename: %s\n",inputFile);
                    
    int file;
    if((file = open(inputFile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
        printf("Unable to create file");
        exit(-4);
    }

    uints *buffer1;
    ulong fileSize1 = lseek(file, 0, SEEK_END);
    buffer1 = (uints*) malloc(sizeof(uints) * fileSize1);

    printf("File: %s --> ",inputFile);
    int num;
    for(int i = 0; i < 100; i++){
        num = (rand() % 100) + 1;
        write(num, buffer1, file);
        printf("%i ",num);
    }
    close(file);

    iteration++;
    //kill(getpid(), SIGUSR1);
}

//----------- ESTO YA FUNCIONA ------------------------------------------------------

void POne(){
  signal(SIGUSR1, ONE);
  while(1);
}

void PTwo(){
  signal(SIGUSR2, TWO);
  while(1);
}

void handler(int signal){
	switch(signal){
		case SIGUSR1:
			printf("ANALYSIS: I received SIGUSR1 from ONE, sending SIGUSR2 to TWO\n");
            kill(pids[1], SIGUSR2);
			break;
		case SIGUSR2:
            if (iteracion < 3){
                iteracion++;
                kill(pids[0], SIGUSR1);
            } else {
                kill(pids[0], SIGINT);
                kill(pids[1], SIGINT);
                exit(0);
            }   
			break;
	}
}

void childs(int i){
    switch(i){
        case 0: ṕrintf("I am ANALYSIS and I create ONE\n");
                POne();
                break;
        case 1: ṕrintf("I am ANALYSIS and I create TWO\n");
                PTwo();
                break;
    }
}

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage: %s timeout prefix_input prefix_output", argv[0]);
		return -1;
	}

    int timeout = atoi(argv[1]);
    if(timeout < 1){
		fprintf(stderr, "%s: the timeout must be a positive integer number\n", argv[0]);
		return -2;
	}

    input = argv[2];
    output = argv[3];

    int pid;
    for(int i = 0; i < 2; i++){
        if ((pid = fork()) < 0){
            perror("Sin hijo");
            exit(-3);
        } else if (pid == 0){
            PROCESS_CHILD(i);
        } else {
            pids[i] = pid;
        }
    }

    srand(SEED);
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
    sleep(timeout);

    printf("ANALYSIS:I send SIGUSR1 to ONE\n");
    kill(pids[0], SIGUSR1);
    wait(NULL);
    
}