#include "header.h"
void a_bee(int id)
{
    key_t key;
    int semid, shmid, i, num;
    if ((key = ftok("/dev/null", 117)) == (key_t) -1)
    {
        perror("ftok");
        exit(-1);
	}

	if ((semid = semget(key, 4, 0666)) < 0)
    {
		perror("semid");
		exit(-1);
	}
	
	if ((shmid = shmget(key, sizeof(struct HoneyBees), 0666)) < 0 )
    {
		perror("shmid");
		exit(-1);
	}
    struct HoneyBees *hb;
    hb = (struct HoneyBees*) shmat(shmid, (void*) 0, 0);
    hb->bees++;
    srand(getpid());
    while (1)
    {
        sleep(2);
        printf("Bee %i: I'm going to put one portion\n", id);
        sem_wait(semid, EMPTY_HONEYPOT, 1);
        sem_wait(semid, MUTEX, 1);
        hb->portions[hb->c_quantity++] = 1;
        if (hb->c_quantity >= hb->honeypot_s)
        {
            printf("Bee %i: Oops! It is full, I will notify the bear.\n", id);
            sem_signal(semid, FULL_HONEYPOT, 1);
        }
        printf("Bee %i: I already put the portion. There are %i portions of %i.\n", id, hb->c_quantity, hb->honeypot_s);
        sem_signal(semid, MUTEX, 1);
        sem_signal(semid, FULL_HONEYPOT, 1);
    }
    shmdt(hb);
    exit(0);
}
int main(int argc, char *argv[])
{
    int n_bees, pid;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s [number_of_bees]\n", argv[0]);
        return -1;
    }
    n_bees = atoi(argv[1]);
    if (n_bees <= 0)
    {
        fprintf(stderr, "%s: [number_of_bees] must be a positive integer.\n", argv[0]);
        return -1;
    }
    for (int i = 0; i < n_bees; i++)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(-1);
        }
        else if (pid == 0)
        {
            a_bee(i);
        }
        else
        {
            // Do nothing...
        }
    }
    
    return 0;
}
