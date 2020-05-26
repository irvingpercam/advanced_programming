/*

* Jose Carlos Pacheco Sanchez - A01702828

* ITESM - QRO - 26 de Mayo 2020 

* EXAMEN No.3 - OSOS y Abejas


* ¿De cuál problema de sincronización se trata?
* R =  Problema de productor consumidor 
*
*
* Define la estrategia y mecanismos que emplearás la solución del problema.
* Fundamenta, de manera clara y concisa, cada una de tus decisiones.
* R = 
*
*/


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

#define NO_DE_PRODUCERS 2 // Momentaneo  para no pasarlo a terminal 
#define BUFFER_MAX 10// Momentaneo para no pasaro a terminal 
#define __deprecated


pthread_t cons;
pthread_t prod[NO_DE_PRODUCERS];

sem_t mutex; 
sem_t cond; 

//Variables gloabales 
bool espacios = false;


int
buffer[BUFFER_MAX],
i = 0,
front = 0,
rear = 0;


void put(int value)
{
    buffer[front] = value;
    front = (front + 1) % BUFFER_MAX;
}

int get()
{
    int tmp = buffer[rear]; 
    rear = (rear + 1) % BUFFER_MAX;
    return tmp;
}

/*
* Usamos nuevamente el "productor" del problema de SANTA
*/
void *producer(void *arg){
  long id = (long) arg;

    while(true){
      sem_wait(&mutex);
      while(espacios){
      sem_post(&mutex);
      sem_wait(&cond);
      sem_wait(&mutex);
    }

    int position = front;
    put(i);
    printf("The bee %ld has put honey number %d \n", id, i);

    i++;

    if(position == (BUFFER_MAX-1)){
      espacios = true;
    }
    sem_post(&mutex);
    sleep(2);

    }
    return 0;

}

/*
* Usamos nuevamente el "Consumidor" del problema de SANTA
*/
void *consumidor(void *arg){
    int i;

    while(true){
      if(espacios){
        sem_wait(&mutex);
        for(i=0;i < BUFFER_MAX; i++){
          get(i);
        }
        printf("Consumer is finshed\n");
        espacios = false;
        sem_post(&cond);
        sem_post(&mutex);
      }
    }

    return 0;
}

int main(int argc, char* argv[]){

    sem_init(&mutex, 0, 1);
    sem_init(&cond, 0, 0);

    pthread_create(&cons, NULL, consumidor, NULL);


    for(long i = 0; i < NO_DE_PRODUCERS; i++){
      pthread_create(&prod[i], NULL, producer, (void *)i);
    }

    pthread_exit(NULL);

    exit(0);

}







