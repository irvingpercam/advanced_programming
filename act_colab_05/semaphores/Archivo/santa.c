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
void santa_claus(int semid){
    printf("Santa Claus is here...\n");
    while(1){
        sem_wait(semid, sem_santa, 1);
        sem_wait(semid, mutex, 1);
        if (reindeer == reindeer_n)
        {
            printf("Santa Claus preparing sleigh...\n");
            for (int i = 0; i < reindeer_n; i++)
            {
                sem_signal(semid, sem_reindeer, 1);
            }
            printf("Happy Christmas\n...");
            reindeer = 0;
        }
        else if (elf == 3)
        {
            printf("Santa Claus helping elves...\n");
        }
        sem_signal(semid, mutex, 1);
    }
}
int main(int argc, char *argv[])
{
    key_t key;
    int semid, i;
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
    santa_claus(semid);
    return 0;
}
