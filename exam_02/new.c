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
/* [END] C Libraries */



/* [START] Constants */
typedef unsigned char mbuff;
#define SEED 12345
char *input_file, *output_file;
int iteration = 1;
/* [END] Constants */

float find_media( ){

	int fd, num, min, i, *data ,numbytes;
	float suma = 0,media;
	unsigned char buffer;

	char inputFile[100], outputFile[100], itr[2], format[10],index[2];
	sprintf(index,"%d",iteration);
	strcat(input_file, index);
	strcat(input_file, ".dat");
	printf("\n\nAbriendo: %s para calcular la media:",input_file);
	if ( (fd = open(input_file, O_RDONLY)) < 0 ) {
		perror("No puedo abrir el archivo");
		return -1;
	}

 	while ((numbytes = read(fd, &buffer, sizeof(char))) > 0){
		suma = suma + buffer;
    }

	 media = suma/100;

	 close(fd);

	return media;
}

float find_variance(float media){

	int fd, num, min, i, *data ,numbytes;
	float sumando = 0,varianza;
	unsigned char buffer;

	char inputFile[100], outputFile[100], itr[2], format[10],index[2];

	printf("\n\nAbriendo: %s para calcular la varianza:",input_file);

	if ( (fd = open(input_file, O_RDONLY)) < 0 ) {
		perror("No puedo abrir el archivo");
		return -1;
	}

 	while ((numbytes = read(fd, &buffer, sizeof(char))) > 0){
		 if(buffer != 0){
		
			sumando = sumando + pow(fabsf(buffer-media),2);
		 }	
    }
	printf("La suma es de: %f",sumando);

	varianza = sumando/99;

	 close(fd);

	return varianza;
}

/* [START] Process One */
void process_one(int signal){
  int file, number, *file_data;
  char itr[2];
  printf("ONE: I received SIGUSR1 from ANALYSIS. I will start to work\n");
  sprintf(itr, "%i", iteration);
  strcat(input_file, itr);
  strcat(input_file, ".dat");
  printf("\nfile: %s -> ", input_file);
  srand(SEED);
  file_data = (int*) malloc(100 * sizeof(int));
  for (int i = 0; i < 100; i++)
  {
    file_data[i] = (rand() % 100) + 1;
    printf("%i", file_data[i]);
  }
  if((file = open(input_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
  {
    perror("can't open file");
    exit(-5);
  }
  write(file, file_data, 100 * sizeof(int));
  close(file);
  free(file_data);
  printf("\nONE: I will send SIGUSR1 to ANALYSIS.\n");
  exit(0);
}
/* [END] Process One */



/* [START] Process Two */
void process_two(){
  printf("TWO: I received SIGUSR2 from ANALYSIS. I will start to work\n");
  int file, *file_data, numbytes;
  char itr[2];
  mbuff buffer;
  sprintf(itr, "%i", iteration);
  strcat(output_file, itr);
  strcat(output_file, ".dat");
  if((file = open(input_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
  {
    perror("can't open file");
    exit(-6);
  }
  file_data = (int*) malloc(100);
  file_data[0] = find_media();
  file_data[1] = find_variance(file_data[1]);
  printf("Mean = %i, Variance = %i", file_data[0], file_data[1]);
  write(file, file_data, sizeof(float)); //16
  close(file);
  printf("\nTWO: I will send SIGUSR2 to ANALYSIS.\n");
  exit(0);
}
/* [END] Process Two */



/* [START] Child Processes */
void child_processes(){
    printf("I am ANALYSIS and I create to ONE.\n");
    signal(SIGUSR1, process_one);
    printf("I am ANALYSIS and I create to TWO.\n");
    signal(SIGUSR2, process_two);
    pause();
    exit(0);
}
/* [END] Child Processes */



/* [START] Main Function */
int main(int argc, char *argv[])
{   /*Function variables */
    int pid, pids[2], timeout;



    /* Test number of parameters */
    if (argc != 4) {
		fprintf(stderr, "usage: %s timeout prefix_input prefix_output\n", argv[0]);
		return -2;
	}


    /* Test timeout (if integer) */
    timeout = atoi(argv[1]);
    if (timeout < 1) {
		fprintf(stderr, "%s: the timeout must be a integer number greater than 0\n", argv[0]);
		return -3;
	}
    input_file = argv[2];
    output_file = argv[3];  

  /* Create processes */
  for (iteration = 1; iteration < 3; iteration++)
  {
    for (int i = 0; i < 2; i++)
    {
      if ((pid = fork()) < 0)
      {
        perror("fork");
        return -4;
      }
      else if (pid == 0)
      {
        child_processes();
      }
      else
      {
        pids[i] = pid;
      }
  }


  /* Send Signals */
  sleep(timeout);
  printf("ANALYSIS: I send SIGUSR1 to ONE.\n");
  kill(pids[0], SIGUSR1);
  sleep(timeout);
  printf("ANALYSIS: I send SIGUSR2 to TWO.\n");
  kill(pids[1], SIGUSR2);
  wait(NULL);
}
return 0;
}
/* [END] Main Function */
