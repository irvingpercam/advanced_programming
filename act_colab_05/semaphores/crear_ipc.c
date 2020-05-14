/*---------------------------------------
* Programación avanzada: Actividad 5
*
* Fecha: 12-05-2020
*
* Autor: Irving Alain Aguilar Pérez - A1703171
*
* Referencias: Ejercicios de https://github.com/Manchas2k4/advanced_programming/tree/master/ipc/semaphores
*----------------------------------------*/
/* [START] C Libraries */
#include "header.h"
int main(int argc, char* argv[]) {
	int semid, i;
	key_t key;
	if (argc != 1)
	{
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	if ((key = ftok("/dev/null", 200)) == (key_t) -1)
	{
		perror(argv[0]);
		return -1;
	}
	if ((semid = semget(key, 4, 0666 | IPC_CREAT)) < 0)
	{
		perror(argv[0]);
		return -1;
	}
	elf = 0;
	reindeer = 0;
	semctl(semid, sem_santa, SETVAL, 0);
	semctl(semid, sem_reindeer, SETVAL, 0);
	semctl(semid, elfTex, SETVAL, 1);
	semctl(semid, mutex, SETVAL, 1);
	return 0;
}
