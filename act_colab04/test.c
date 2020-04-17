/*-----------------------------------------
* Programación avanzada: Actividad 4
* Fecha: 17-04-2020
* Autor: Raymundo Romero Arenas - A00570654
* Autor: Alonso Oropeza Arévalo - A01703424
*----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#define MAX_SIZE 10000

char content1[MAX_SIZE];
char content2[MAX_SIZE];
char content3[MAX_SIZE];
char *pch1;
char *pch2;
char *pch3;
char *pch4;
char *pch5;
char *pch6;

// Ejecutar el comando según la señal
void handler(int signal){
	int pid;
	switch(signal){
		case SIGUSR1:
			if ((pid = fork()) < 0) {
				perror("Sin proceso hijo");
			} else if (pid == 0) {
				execlp(pch1,pch2,NULL);
			}
			sleep(3);
			break;
		case SIGUSR2:
			if ((pid = fork()) < 0) {
				perror("Sin proceso hijo");
			} else if (pid == 0) {
				execlp(pch3,pch4,NULL);
			}
			sleep(3);
			break;
		case SIGPWR:
			if ((pid = fork()) < 0) {
				perror("Sin proceso hijo");
			} else if (pid == 0) {
				execlp(pch5,pch6,NULL);
			}
			sleep(3);
			break;
		case SIGINT:
            puts("Ending...");
			_exit(0);
	}
}

// Cargar los comandos del archivo
int load(FILE* FD, char* buffer){
	if (FD != NULL){
		fseek(FD, 0L, SEEK_END);
		long pos = ftell(FD);
		rewind(FD);
		if(buffer != NULL) {
			fread(buffer, pos, 1, FD);
			fclose(FD); 
            FD = NULL;
		}
		if(FD != NULL) {
            fclose(FD);
        }
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	struct stat st;
	int size;

	if(argc != 4){
		fprintf(stderr, "usage: %s file1 file2 file3\n", argv[0]);
		return -2;
	}

	// File1.txt
	FILE* F1 = fopen(argv[1], "r");
	if(!F1){
		printf("File %s does not exist\n", argv[1]);
		return -3;
	}

	stat(argv[1], &st);
	size = st.st_size;
	if(size <= 0){
		printf("File %s is empty\n", argv[1]);
		return -6;
	}
	load(F1,content1);
    pch1 = strtok(content1, ",");
	pch2 = strtok(NULL, ",");

	// File2.txt
	FILE* F2 = fopen(argv[2], "r");
	if(!F2){
		printf("File %s does not exist\n", argv[2]);
		return -4;
	}

	stat(argv[2], &st);
	size = st.st_size;
	if(size <= 0){
		printf("File %s is empty\n", argv[2]);
		return -7;
	}
	load(F2, content2);
    pch3 = strtok(content2, ",");
	pch4 = strtok(NULL, ",");

	// File3.txt
	FILE* F3 = fopen(argv[3], "r");
	if(!F3){
		printf("File %s does not exist\n", argv[3]);
		return -5;
	}

	stat(argv[3], &st);
	size = st.st_size;
	if(size <= 0){
		printf("File %s is empty\n", argv[3]);
		return -8;
	}
	load(F3, content3);
    pch5 = strtok(content3, ",");
	pch6 = strtok(NULL, ",");

	// Señales
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	signal(SIGPWR, handler);
	signal(SIGINT, handler);

	puts("Waiting for a signal...");

	while(1);
	return 0;
}