#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM 100000000

int massiv[NUM];
double result[10];

struct params{
    int index;
    int start;
    int finish;
};

void *mythreadM(void *dummy)
{
    struct params *ptr = (struct params*)(dummy);
    for (int i = ptr->start; i < ptr->finish + 1; i++){
        result[ptr->index] = result[ptr->index] + massiv[i]; 
    }
    result[ptr->index] = 4 * result[ptr->index] / NUM;
        for (int i = ptr->start; i < ptr->finish + 1; i++){
        result[ptr->index + 1] = result[ptr->index + 1] + (massiv[i] - result[ptr->index]) * (massiv[i] - result[ptr->index]); 
    }
    return NULL;
}

int main(int argc, char* argv[], char* envp[])
{
    int ON_THREADS = atoi(argv[1]);
    int i;
    srand(time(NULL));
    for (i = 0; i < NUM ; i++ ){
        massiv[i] = rand() % 50 ;
    }
    for (i = 0; i < 10; i++)
        result[i] = 0;
    if (ON_THREADS == 0) {
        clock_t begin = clock();
        for (i = 0; i < NUM; i++){
            result[8] = result[8] + (double)massiv[i];
        }
        result[8] = result[8] / (double)NUM; 
        for (i = 0; i < NUM; i++){
            result[9] = result[9] + (result[8] - (double)massiv[i]) * (result[8] - (double)massiv[i]);
        }
        result[9] = result[9] / NUM;
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("НЕ НИТИ\nМАТОЖИДАНИЕ = %lg\nДИСПЕРСИЯ = %lg\nВРЕМЯ = %lg\n", result[8], result[9], time_spent); 
    }
    else {
        clock_t begin = clock();
        pthread_t mthid_0, mthid_1, mthid_2, mthid_3;
        struct params param[4];
        for (i = 0; i < 4; i++){
            param[i].index = 2 * i;
            param[i].start = i * NUM / 4;
            param[i].finish = (i + 1) * NUM / 4 - 1;
            //printf("index %d start %d finish %d\n", param[i].index, param[i].start, param[i].finish); 
        }
      
        pthread_create( &mthid_0, (pthread_attr_t *)NULL, mythreadM, (void *)param);
        pthread_create( &mthid_1, (pthread_attr_t *)NULL, mythreadM, (void *)(param + 1));
        pthread_create( &mthid_2, (pthread_attr_t *)NULL, mythreadM, (void *)(param + 2));
        pthread_create( &mthid_3, (pthread_attr_t *)NULL, mythreadM, (void *)(param + 3));
        pthread_join(mthid_0, (void **)NULL);
        pthread_join(mthid_1, (void **)NULL);
        pthread_join(mthid_2, (void **)NULL);
        pthread_join(mthid_3, (void **)NULL);
        result[8] = (result[0] + result[2] + result[4] + result[6]) / 4;
        result[9] = (result[1] + result[3] + result[5] + result[7]) / NUM;
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("НИТИ\nМАТОЖИДАНИЕ = %lg\nДИСПЕРСИЯ = %lg\nВРЕМЯ = %lg\n", result[8], result[9], time_spent);
    }
    return 0;
}
