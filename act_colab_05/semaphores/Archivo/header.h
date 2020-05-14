#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define reindeer_n 	9
#define	sem_santa 	0
#define sem_reindeer	1
#define	elfTex 		2
#define mutex 		3
int elf;
int reindeer;
int sem_wait(int semid, int sem_num, unsigned int val) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}
int sem_signal(int semid, int sem_num, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}
#endif