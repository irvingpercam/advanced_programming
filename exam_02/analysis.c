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
#define SEED 12345
typedef unsigned char mbuff;
char *input_file,*output_file;
int iteration = 1;
/* [END] Constants */

/* [START] Process One */
void p_one(int signal){
  int file, i, number, *file_data;
  char itr[2];
	printf("ONE: I received SIGUSR1 from ANALYSIS. I will start to work\n");
  sprintf(itr,"%d",iteration);
  strcat(input_file, itr);
  strcat(input_file, ".dat");
  printf("\nfile: %s -> ", input_file);
  srand(SEED);
  file_data = (int*) malloc(100 * sizeof(int));
  for (i = 0; i < 100; i++)
  {
    file_data[i] = rand() % (100) + 1;
    printf(" %i ", file_data[i]);
  }
  if ((file = open(input_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
  {
    perror("can't open file");
    exit(-5);
  }
  write(file, file_data, 100 * sizeof(int));
  close(file);
	exit(0);
}
/* [END] Process One */



/* [START] Process Two */
void p_two(int signal){
  int file, file2, *file_data, n_data;
  char itr[2], in_media[2];
  float sum = 0, media = 0, variance;
  mbuff buff, media_buff, var_buff;
  printf("TWO: I received SIGUSR2 from ANALYSIS. I will start to work\n");
  sprintf(itr,"%d",iteration);
  strcat(output_file, itr);
  strcat(output_file, ".dat");
  if ((file = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
  {
    perror("can't open file");
    exit(-6);
  }
  // For media
	sprintf(in_media,"%i",iteration);
	strcat(input_file, in_media);
	strcat(input_file, ".dat");
	if ((file2 = open(input_file, O_RDONLY)) < 0 )
  {
		perror("can't open file");
		exit(-7);
	}
  while ((n_data = read(file2, &media_buff, sizeof(char))) > 0)
  {
		sum = sum + media_buff;
  }
	media = sum/100;
  close(file2);
  // For variance
  sum = 0;
  printf("reading file: %s", input_file);
	if ((file = open(input_file, O_RDONLY)) < 0)
  {
		perror("can't open file");
		exit(-8);
	}
 	while ((n_data = read(file, &var_buff, sizeof(char))) > 0){
    if(var_buff != 0)
    {
    sum = sum + pow(fabsf(var_buff - media),2);
    }	
  }
	variance = sum/99;
  file_data = (int*) malloc(100);
  file_data[0] = media;
  file_data[1] = variance;
  printf("\ncalculating avg and stddev\n");
  printf("\nresults... avg = %i, stddev = %i\n", file_data[0], file_data[1]);
  printf("\nwriting to file: %s\n", output_file);
  write(file, file_data, sizeof(float));
  close(file);
  printf("TWO: I send SIGUSR2 to ANALYSIS.\n");
  exit(0);
}
/* [END] Process Two */



/* [START] Child Processes */
void c_processes() {
	signal(SIGUSR1, p_one);
	signal(SIGUSR2, p_two);
	pause();
	exit(0);
}
/* [END] Child Processes */



/* [START] Main Function */
int main(int argc, char *argv[])
{   /*Function variables */
	int pid,pids[2],timeout, p_count = 1;
  input_file = argv[2];
	output_file = argv[3];


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


	for (iteration = 1;iteration <= 3 ;iteration++)
  {
		for (int i = 0; i < 2; i++)
    {
      if ((pid = fork()) < 0)
      {
        perror("fork");
      }
      else if (pid == 0)
      {
        signal(SIGUSR1, p_one);
	      signal(SIGUSR2, p_two);
        while(1);
      }
      else
      {
        pids[i] = pid;
        printf("I am ANALYSIS and I create to %d.\n", p_count);
        p_count++;
      }
		}
		sleep(timeout);
		printf("\nANALYSIS: I send SIGUSR1 to ONE\n");
		kill(pids[0], SIGUSR1);
		sleep(timeout);
		printf("\nANALYSIS: I received SIGUSR1 from ONE, sending SIGUSR2 to TWO\n");
		kill(pids[1], SIGUSR2);
		wait(NULL);
	}
return 0;
}
/* [END] Main Function */
