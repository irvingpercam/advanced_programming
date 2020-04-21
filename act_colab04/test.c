#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
typedef unsigned long ulong;
char cmd1[100];
char cmd2[100];
char cmd3[100];
char *l1;
char *l2;
char *l3;
char *l4;
char *l5;
char *l6;

// Ejecutar el comando según la señal
void handler(int signal){
	int pid;
	switch(signal){
		case SIGUSR1:
			if ((pid = fork()) < 0) {
				perror("fork");
			} else if (pid == 0) {
				execlp(l1,l2,NULL);
			}
			sleep(3);
			break;
		case SIGUSR2:
			if ((pid = fork()) < 0) {
				perror("fork");
			} else if (pid == 0) {
				execlp(l3,l4,NULL);
			}
			sleep(3);
			break;
		case SIGPWR:
			if ((pid = fork()) < 0) {
				perror("fork");
			} else if (pid == 0) {
				execlp(l5,l6,NULL);
			}
			sleep(3);
			break;
		case SIGINT:
        	puts("Ending...");
			_exit(0);
	}
}

// Cargar los comandos del archivo
int load(FILE* file, char* buffer){
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

int main(int argc, char** argv) {
	ulong f_size;
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
	f_size = lseek(F1, 0, SEEK_END);
	if (f_size < 1)
	{
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[1]);
		return -6;
	}
	load(F1,cmd1);
    l1 = strtok(cmd1, ",");
	l2 = strtok(NULL, ",");

	// File2.txt
	FILE* F2 = fopen(argv[2], "r");
	if(!F2){
		printf("File %s does not exist\n", argv[2]);
		return -4;
	}
	f_size = lseek(F2, 0, SEEK_END);
	if (f_size < 1)
	{
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[2]);
		return -7;
	}
	load(F2, cmd2);
    l3 = strtok(cmd2, ",");
	l4 = strtok(NULL, ",");

	// File3.txt
	FILE* F3 = fopen(argv[3], "r");
	if(!F3){
		printf("File %s does not exist\n", argv[3]);
		return -5;
	}
	f_size = lseek(F3, 0, SEEK_END);
	if (f_size < 1)
	{
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[3]);
		return -8;
	}
	load(F3, cmd3);
    l5 = strtok(cmd3, ",");
	l6 = strtok(NULL, ",");

	// Señales
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	signal(SIGPWR, handler);
	signal(SIGINT, handler);

	puts("Waiting for a signal...");

	while(1);
	return 0;
}