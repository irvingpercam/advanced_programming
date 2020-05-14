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
void elfo(int num, int semid){
    int helpElve;
    while(1){
        helpElve = (rand() % num) + 1;
        printf("This is elf #%d\n", helpElve);
        helpElve = rand() % 1;
		if (helpElve == 1)
        {
			sem_wait(semid, elfTex, 1);
			sem_wait(semid, mutex, 1);
			elf++;
			if (elf == 3)
            {
				sem_signal(semid, sem_santa, 1);
            }
            else
            {
				sem_signal(semid, elfTex, 1);
            }
			sem_signal(semid, mutex, 1);
			printf("Elf #%d needs help...\n", num);
			sleep(2);
			sem_wait(semid, mutex, 1);
			elf--;
			if (elf == 0)
            {
				sem_signal(semid, elfTex, 1);
            }
			sem_signal(semid, mutex, 1);
		}
		printf("Elf #%d back to work...\n", num);
		sleep(rand() % 3);
    }
}
int main(int argc, char *argv[])
{
    key_t key;
    int semid, i;
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
    elf = atoi(argv[1]);
    elfo(elf, semid);
    return 0;
}
