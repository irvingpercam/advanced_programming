#include "header.h"
int main(int argc, char *argv[])
{
    key_t key;
	int semid, i, shmid;
	if (argc != 1)
    {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}
	if ((key = ftok("/dev/null", 117)) == (key_t) -1)
    {
		perror(argv[0]);
		return -1;
	}
	if ((semid = semget(key, 3, 0666)) < 0)
    {
		perror(argv[0]);
		return -1;
	}
	if ((shmid = shmget(key, sizeof(struct HoneyBees), 0666)) < 0 )
    {
		perror("shmid");
		return -1;
	}
    struct HoneyBees *hb;
    hb = (struct HoneyBees*) shmat(shmid, (void*) 0, 0);
    printf("Bear: The jar is empty. I will wait for honey ... while I sleep.\n");
    while (1)
    {
        sem_wait(semid, FULL_HONEYPOT, 1);
        sem_wait(semid, MUTEX, 1);
        if (hb->c_quantity == hb->honeypot_s)
        {
            printf("Bear: What beautiful bees! The jar is full! Now, I'm going to eat!\n");
            sem_signal(semid, EMPTY_HONEYPOT, hb->honeypot_s);
            hb->c_quantity = 0;
            printf("Bear: The jar is empty. I will wait for honey ... while I sleep.\n");
        }
        sem_signal(semid, MUTEX, 1);
    }
    shmdt(hb);
    semctl(semid, 0, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}
