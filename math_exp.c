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
double result[6];

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
    return NULL;
}
void *mythreadD(void *dummy)
{
    struct params *ptr = (struct params*)(dummy);
    result[ptr->index] = 0;
    for (int i = ptr->start; i < ptr->finish + 1; i++){
        result[ptr->index] = result[ptr->index] + (massiv[i] - result[4]) * (massiv[i] - result[4]); 
    }
    return NULL;
}

int main(int argc, char* argv[], char* envp[])
{
    int ON_THREADS = atoi(argv[1]);
    int i;
    //srand(time(NULL));
    for (i = 0; i < NUM ; i++ ){
        massiv[i] = rand() % 50 ;
    }
    for (i = 0; i < 6; i++)
        result[i] = 0;
    if (ON_THREADS == 0) {
        clock_t begin = clock();
        for (i = 0; i < NUM; i++){
            result[4] = result[4] + (double)massiv[i];
        }
        result[4] = result[4] / (double)NUM; 
        for (i = 0; i < NUM; i++){
            result[5] = result[5] + (result[4] - (double)massiv[i]) * (result[4] - (double)massiv[i]);
        }
        result[5] = result[5] / NUM;
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("НЕ НИТИ\nМАТОЖИДАНИЕ = %lg\nДИСПЕРСИЯ = %lg\nВРЕМЯ = %lg\n", result[4], result[5], time_spent); 
    }
    else {
        clock_t begin = clock();
        pthread_t mthid_0, mthid_1, mthid_2, mthid_3, dthid_0, dthid_1, dthid_2, dthid_3;
        struct params param[4];
        for (i = 0; i < 4; i++){
            param[i].index = i;
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
        result[4] = (result[0] + result[1] + result[2] + result[3]) / NUM;
        if (result[4]){
            pthread_create( &dthid_0, (pthread_attr_t *)NULL, mythreadD, (void *)param);
            pthread_create( &dthid_1, (pthread_attr_t *)NULL, mythreadD, (void *)(param + 1));
            pthread_create( &dthid_2, (pthread_attr_t *)NULL, mythreadD, (void *)(param + 2));
            pthread_create( &dthid_3, (pthread_attr_t *)NULL, mythreadD, (void *)(param + 3));
            pthread_join(dthid_0, (void **)NULL);
            pthread_join(dthid_1, (void **)NULL);
            pthread_join(dthid_2, (void **)NULL);
            pthread_join(dthid_3, (void **)NULL);
            result[5] = (result[0] + result[1] + result[2] + result[3]) / NUM;
        }
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("НИТИ\nМАТОЖИДАНИЕ = %lg\nДИСПЕРСИЯ = %lg\nВРЕМЯ = %lg\n", result[4], result[5], time_spent); 
    }
    return 0;
}
