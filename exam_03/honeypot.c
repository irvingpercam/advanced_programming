#include "header.h"
int main(int argc, char *argv[])
{
    int semid, shmid, honeypot_size;
    key_t key;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s [honeypot_size]\n", argv[0]);
        return -1;
    }
    honeypot_size = atoi(argv[1]);
    if ((key = ftok("/dev/null", 200)) == (key_t) -1)
    {
        perror(argv[0]);
        return -1;
    }
    if ((semid = semget(key, 3, 0666 | IPC_CREAT)) < 0)
    {
        perror(argv[0]);
        return -1;
    }
    semctl(semid, FULL_HONEYPOT, SETVAL, 0);
	semctl(semid, EMPTY_HONEYPOT, SETVAL, honeypot_size);
	semctl(semid, MUTEX, SETVAL, 1);

	if ((shmid = shmget(key, sizeof(struct HoneyBees), 0666 | IPC_CREAT)) < 0) 
    {
		semctl(semid, 0, IPC_RMID, 0);
		perror("shmid");
		return -1;
	} 	
    struct HoneyBees * hb;
    hb = (struct HoneyBees*) shmat(shmid, (void*) 0, 0);
    hb->c_quantity = 0;
    hb->honeypot_s = honeypot_size;
    memset(hb->portions, 0, honeypot_size * sizeof(int));
    shmdt(hb);
    printf("Semaphores created, variables initialized...\n");
    return 0;
}
