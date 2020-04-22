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

const char *input;
const char *output;
int iteration = 1;
int pids[2];

void TWO(){
    printf("TWO: I received SIGUSR2 from ANALYSIS. I will start to work");

    char inputFile[100], outputFile[100], itr[2];
    int size = sprintf(itr,"%i",iteration);

    strcpy(inputFile, input);
    strcat(inputFile, itr);
    strcat(inputFile, ".dat");

    strcpy(outputFile, output);
    strcat(outputFile, itr);
    strcat(outputFile, ".dat");

    int file1;
    if((file1 = open(inputFile, O_RDONLY)) < 0){
        printf("Unable to open file");
        exit(-5);
    }

    int num;
    int nums[105];
    int f1d;
    int sum = 0;
    int i = 0;
    while((f1d = read(file1, &num, sizeof(int))) != 0){
        sum = sum + f1d;
        nums[i] = f1d;
         i++;
    }
    float mean = sum/100;

    float acum = 0;
    for(int i = 0; i<100; i++){
        acum = acum + pow(fabsf(nums[i]-mean),2);
    }
    float variance = acum/(99);

    printf("Mean = %f, Variance = %f",mean,variance);

    int file2;
    if((file2 = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
        printf("Unable to create file");
        exit(-6);
    }
    write(file2, &mean, sizeof(float));
    write(file2, &variance, sizeof(float));
    close(file1);
    close(file2);
    iteration++;
    printf("TWO: I send SIGUSR2 to ANALYSIS.\n");
    kill(getppid(), SIGUSR2);
}

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

    printf("File: %s --> ",inputFile);
    int num;
    srand(SEED);
    for(int i = 0; i < 100; i++){
        num = (rand() % 100) + 1;
        write(file, &num, sizeof(int));
        printf("%i ",num);
    }
    close(file);

    iteration++;
    printf("ONE: I send SIGUSR1 to ANALYSIS.\n");
    kill(getppid(), SIGUSR1);
}

void P1(){
  signal(SIGUSR1, ONE);
  while(1);
}

void P2(){
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
            if (iteration < 3){
                iteration++;
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
        case 0: printf("I am ANALYSIS and I create ONE\n");
                P1();
                break;
        case 1: printf("I am ANALYSIS and I create TWO\n");
                P2();
                break;
    }
}

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage: %s timeout prefix_input prefix_output\n", argv[0]);
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
            childs(i);
        } else {
            pids[i] = pid;
        }
    }

	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
    sleep(timeout);

    printf("ANALYSIS:I send SIGUSR1 to ONE\n");
    kill(pids[0], SIGUSR1);
    wait(NULL);
    
}