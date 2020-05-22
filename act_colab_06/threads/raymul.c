/*-----------------------------------------
* Programación avanzada: Actividad 6
* Fecha: 17-04-2020
* Autor: Raymundo Romero Arenas - A00570654
* Autor: Alonso Oropeza Arévalo - A01703424
*----------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

int sizeA, sizeB, threads_amount;
float **MA, **MB, **MC;

void memory(int sizeA){
    MA = malloc(sizeA * sizeof(float*)); 
    for (int i = 0; i < sizeA; i++) {
        MA[i] = malloc(sizeA * sizeof(float));
    }
    
    MB = malloc(sizeB * sizeof(float*)); 
    for (int i = 0; i < sizeB; i++) {
        MB[i] = malloc(sizeA * sizeof(float));
    }

    MC = malloc(sizeA * sizeof(float*)); 
    for (int i = 0; i < sizeA; i++) {
        MC[i] = malloc(sizeA * sizeof(float));
    }
}

void *calculate(void *arg){
    int tid, rows, start, end;
    float sum;
    int rem = sizeA % threads_amount;
  
    tid = *(int* )(arg);
    rows = ceil(sizeA / threads_amount);
    start = tid * rows;
    end = (tid + 1) * rows;

    if(tid == threads_amount - 1){
      end = end + rem;
    }

    for (int i = start; i < end; i++) { 
        for (int j = 0; j < sizeA; j++) { 
            sum = 0;
            for (int k = 0; k < sizeA; k++) { 
                sum = sum + MA[i][k] * MB[k][j];
            }
            MC[i][j] = sum;
        }
    }
}

int main(int argc, char* argv[]){
    int fileA, fileB;
    double sum = 0;
    double time = 0;
    pthread_t *threads;

    if (argc != 7) {
        printf("%s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
        return -2;
    }

    if (strcmp(argv[1], "-a") != 0) {
        printf("%s no es una bandera válida para la 1° matriz\n", argv[1]);
        return -3;
    }
    if (strcmp(argv[3], "-b") != 0) {
        printf("%s no es una bandera válida para la 2° matriz\n", argv[3]);
        return -4;
    }
    if (strcmp(argv[5], "-t") != 0) {
        printf("%s no es una bandera válida para los threads\n", argv[5]);
        return -5;
    }

    if ((fileA = open(argv[2], O_RDONLY)) < 0) {
        printf("%s (matrix_file_a) no es archivo válido\n", argv[2]);
        return -6;
    } 
    if ((fileB = open(argv[4], O_RDONLY)) < 0) {
        printf("%s (matrix_file_b) no es archivo válido\n", argv[4]);
        return -7;
    }

    threads_amount = atoi(argv[6]);
    if(threads_amount <= 0){
        printf("El número de threads debe ser un entero positivo\n");
        return -8;
    }

    FILE *FA, *FB;
    FA = fopen(argv[2],"r");
    FB = fopen(argv[4],"r");

    if((FA = fopen(argv[2],"r")) != NULL && (FB = fopen(argv[4],"r")) != NULL) {
        fscanf(FA,"%d",&sizeA);
        fscanf(FB,"%d",&sizeB);
    }
    if(sizeA == sizeB){
        memory(sizeA);
        for(int i = 0; i < sizeA; i++){
            for(int j = 0; j < sizeA; j++){
                if(!fscanf(FA, "%f", &MA[i][j])){
                    break;
                }
            }
        }
        for(int i = 0; i < sizeB; i++){
            for(int j = 0; j < sizeB; j++){
                if(!fscanf(FB, "%f", &MB[i][j])){
                    break;
                }
            }
        }
        fclose(FA);
        fclose(FB);
    } else {
        printf("Las matrices deben ser del mismo tamaño\n");
        return -9;
    }

    threads = (pthread_t*) malloc(threads_amount * sizeof(pthread_t));
    
    clock_t begin = clock();
    for (int i = 0; i < threads_amount; i++) {
        int *tid;
        tid = (int *) malloc(sizeof(int));
        *tid = i;
        pthread_create(&threads[i], NULL, calculate, (void *)tid);
    } 
    for (int i = 0; i < threads_amount; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_t end = clock();

    //Imprime el resultado de la multiplicación
    //DESCOMENTAR SOLO SI QUIERES VER LA MATRIZ EN TERMINAL
    
    printf("Matriz resultante: \n");
    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            printf("%f ", MC[i][j]);
        }
        printf("\n");
    }
    

    time += (double)(end - begin) / CLOCKS_PER_SEC;   
    printf("Número de threads: %i, Tiempo de ejecucion: %.4f s\n",threads_amount, time/threads_amount);
}

/*-----------------------------------------
* Tiempos de ejecución
* Archivos usados: a1181.txt, b1181.txt
* 1 thread = 12.8430s
* 2 threads = 6.7849s
* 3 threads = 5.9356s
* 5 threads = 5.3004s
* 10 threads = 2.8937s
* 20 threads = 1.3974s
* 50 threads = 0.5514s
* 100 threads = 0.2692s
*----------------------------------------*/
