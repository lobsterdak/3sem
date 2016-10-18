#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/* Время без  нитей = 0.237643
 * Время с 4 нитями = 0.106461*/


#define NUM 100000000
#define NUM_OF_THREADS 4

struct timespec start, finish;
double elapsed;

int numbers[NUM];

/*
 * Я бы сделал два массива: averages и dispersions, либо sum и squareSum, вместо одного массива results,
 * где в чётных позициях хранится сумма элементов, а в нечетных - сумма квадратов.
 * 
 * Вы не выигрываете лаконичности кода, зато проигрываете в читаемости.
 */
double result[2 * (NUM_OF_THREADS + 1)];

struct params{
    int index;
    int start;
    int finish;
};

void *mythreadM(void *dummy)
{
    struct params *ptr = (struct params*)(dummy);
    for (int i = ptr->start; i < ptr->finish + 1; i++){
        result[ptr->index] = result[ptr->index] + numbers[i];
        result[ptr->index + 1] = result[ptr->index + 1] + numbers[i] * numbers[i]; 
    }
    result[ptr->index] = result[ptr->index] / NUM;
    result[ptr->index + 1] = result[ptr->index + 1] / NUM;
    return NULL;
}

int main(int argc, char* argv[], char* envp[])
{
    int ON_THREADS = atoi(argv[1]);
    int i;
    //srand(time(NULL));
    for (i = 0; i < NUM ; i++ ){
        numbers[i] = rand() % 50 ;
    }
    for (i = 0; i < 2 * (NUM_OF_THREADS + 1); i++)
        result[i] = 0;
    if (ON_THREADS == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (i = 0; i < NUM; i++){
            result[2 * NUM_OF_THREADS] = result[2 * NUM_OF_THREADS] + (double)numbers[i];
        }
        result[2 * NUM_OF_THREADS] = result[2 * NUM_OF_THREADS] / NUM; 
        for (i = 0; i < NUM; i++){
            result[2 * NUM_OF_THREADS + 1] = result[2 * NUM_OF_THREADS + 1] + (result[2 * NUM_OF_THREADS] - (double)numbers[i]) * (result[2 * NUM_OF_THREADS] - (double)numbers[i]);
        }
        result[2 * NUM_OF_THREADS + 1] = result[2 * NUM_OF_THREADS + 1] / NUM;
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("НЕ НИТИ\nМАТОЖИДАНИЕ = %lg\nДИСПЕРСИЯ = %lg\nВРЕМЯ = %lg\n", result[8], result[9], elapsed); 
    }
    else {
        clock_gettime(CLOCK_MONOTONIC, &start);
        pthread_t mthid[NUM_OF_THREADS];
        struct params param[NUM_OF_THREADS];
        for (i = 0; i < NUM_OF_THREADS; i++){
            param[i].index = 2 * i;
            param[i].start = i * NUM / NUM_OF_THREADS;
            param[i].finish = (i + 1) * NUM / NUM_OF_THREADS - 1;
        }
        for (i = 0; i < NUM_OF_THREADS; i++){
            pthread_create( &mthid[i], (pthread_attr_t *)NULL, mythreadM, (void *)(param + i));
        }
        for (i = 0; i < NUM_OF_THREADS; i++) {
            pthread_join(mthid[i], (void **)NULL);
        }
        for (i = 0; i < NUM_OF_THREADS; i++) {
            result[2 * NUM_OF_THREADS] = result[2 * NUM_OF_THREADS] + result[2 * i];
            result[2 * NUM_OF_THREADS + 1] = result[2 * NUM_OF_THREADS + 1] + result[2 * i + 1];
        }
        result[2 * NUM_OF_THREADS + 1] = result[2 * NUM_OF_THREADS + 1] - result[2 * NUM_OF_THREADS] * result[2 * NUM_OF_THREADS] ;
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("НИТИ\nМАТОЖИДАНИЕ = %lg\nДИСПЕРСИЯ = %lg\nВРЕМЯ = %lg\n", result[2 * NUM_OF_THREADS], result[2 * NUM_OF_THREADS + 1], elapsed); 
    }
    return 0;
}
