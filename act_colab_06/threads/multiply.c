/*---------------------------------------
* Programación avanzada: Actividad 6
*
* Fecha: 22-05-2020
*
* Autor: Irving Alain Aguilar Pérez - A1703171
*
* Referencias: Ejercicios de https://github.com/Manchas2k4/advanced_programming/tree/master/
*----------------------------------------*/
/*
*   TESTS
*   The tests where performed with the given input files (a1181.txt and b1181.txt).
*   threads: 1, execution time: 15.911314sec
*   threads: 2, execution time: 7.381496sec
*   threads: 3, execution time: 6.989684sec
*   threads: 5, execution time: 4.870440sec
*   threads: 10, execution time: 2.368243sec
*   threads: 20, execution time: 1.369382sec
*   threads: 50, execution time: 0.477704sec
*   threads: 100, execution time: 0.222946sec
*/
/* [START] C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
float **m_matrix_a;
float **m_matrix_b;
float **m_matrix_c;
int n_threads;
int size;
void memory_allocation(int size){
    m_matrix_a = malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++)
    {
    m_matrix_a[i] = malloc(size * sizeof(float));
    }
    m_matrix_b = malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++)
    {
        m_matrix_b[i] = malloc(size * sizeof(float));
    }
    m_matrix_c = malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++)
    {
        m_matrix_c[i] = malloc(size * sizeof(float));
    }
}
void *multiply(void *arg){
    int thid = *(int*)(arg), st, en, rows, rem_rows;
    float sum;
    rows = ceil(size / n_threads);
    st = thid * rows;
    en = (thid + 1) * rows;
    if (thid == n_threads - 1)
    {
        en += rem_rows;
    }
    for (int i = st; i < en; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sum = 0;
            for (int k = 0; k < size; k++)
            {
                sum += m_matrix_a[i][k] * m_matrix_b[k][j];
            }
            m_matrix_c[i][j] = sum;
        }
    }
}
void fill_matrices(FILE *matrix_a, FILE *matrix_b, float **m_matrix_a, float **m_matrix_b, int size){
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fscanf(matrix_a, "%f", &m_matrix_a[i][j]);
        }
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fscanf(matrix_b, "%f", &m_matrix_b[i][j]);
        }
    }
}
void result(){
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%f", m_matrix_c[i][j]);
        }
        puts("\n");
    }
}
int main(int argc, char *argv[])
{
    int in_file, out_file, size_matrix_a, size_matrix_b;
    FILE *matrix_a, *matrix_b;
    pthread_t *threads;
    clock_t start_time, end_time;
    double time;
    if (argc != 7) {
		fprintf(stderr, "usage: %s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
		return -2;
	}
    if (strcmp(argv[1], "-a") != 0)
    {
        fprintf(stderr, "usage: %s, the %s flag is not valid, please use the -a flag instead\n", argv[0], argv[1]);
        return -3;
    }
    if (strcmp(argv[3], "-b") != 0)
    {
        fprintf(stderr, "usage: %s, the %s flag is not a valid argument, please use the -b flag instead\n", argv[0], argv[1]);
        return -4;
    }
    if (strcmp(argv[5], "-t") != 0)
    {
        fprintf(stderr, "usage: %s, the %s flag is not valid, please use the -t flag instead\n", argv[0], argv[1]);
        return -5;
    }
    if ((in_file = open(argv[2], O_RDONLY)) < 0)
    {
        fprintf(stderr, "usage: %s, the %s file is not valid.\n", argv[0], argv[2]);
        return -6;
    }
    if ((out_file = open(argv[4], O_RDONLY)) < 0)
    {
        fprintf(stderr, "usage: %s, the %s file is not valid.\n", argv[0], argv[4]);
        return -7;
    }
    n_threads = atoi(argv[6]);
    if (n_threads <= 0)
    {
        fprintf(stderr, "usage: %s, the %s parameter is not valid... parameter should be a positive integer.\n", argv[0], argv[6]);
        return -8;
    }
    matrix_a = fopen(argv[2], "r");
    matrix_b = fopen(argv[4], "r");
    fscanf(matrix_a, "%i", &size_matrix_a);
    fscanf(matrix_b, "%i", &size_matrix_b);
    if (size_matrix_a != size_matrix_b)
    {
        fprintf(stderr, "usage: %s, matrices %s and %s must be the same size.\n", argv[0], argv[2], argv[4]);
        return -9;
    }
    else
    {
        size = size_matrix_a;
        memory_allocation(size_matrix_a);
        fill_matrices(matrix_a, matrix_b, m_matrix_a, m_matrix_b, size);
        fclose(matrix_a);
        fclose(matrix_b);
    }
    threads = (pthread_t*)malloc(n_threads * sizeof(pthread_t));
    start_time = clock();
    for (int i = 0; i < n_threads; i++)
    {
        int *thid;
        thid = (int*)malloc(sizeof(int));
        *thid = i;
        pthread_create(&threads[i], NULL, multiply, (void *)thid);
    }
    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    end_time = clock();
    result();
    time += ((double)(end_time - start_time)/CLOCKS_PER_SEC)/n_threads;
    printf("threads: %i, execution time: %fsec\n", n_threads, time);
    free(m_matrix_a);
    free(m_matrix_b);
    return 0;
}
