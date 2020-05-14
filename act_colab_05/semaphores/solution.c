#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
int main(int argc, char* argv[]){
	int num;
	if (argc != 1) {
		fprintf(stderr, "usage: %s buffer_size\n", argv[0]);
		return -1;
	}
	num = atoi(argv[1]);
    if (num <= 1) {
		fprintf(stderr, "%s: the parameter must be a positive integer number\n", argv[0]);
		return -3;
	}
    return 0;
}