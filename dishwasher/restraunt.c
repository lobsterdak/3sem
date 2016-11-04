#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <pthread.h>

#define TABLE_LIMIT 2
#define BIG_CONST 100


typedef struct descripsion_of_operations {
    char *name;
    int time;
} desc;

typedef struct dirty_dish {
    char *name;
    int num;
} dish;

typedef struct slave {
    desc *ptr;
    int amount;
} slave;

dish *list;
int AMOUNT; //numbers of kinds dish
int semid;

char *input_information(const char *file_name, int *num_of_tokens) {
    FILE *file;
    file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *str = (char *)malloc(sizeof(char) *file_size);
    fread(str, sizeof(char), file_size, file);
    fclose(file);
    int i = 0;
    (*num_of_tokens) = 0;
    while (str[i]) {
        if(str[i] == '\n')
            (*num_of_tokens)++ ;
        i++;
    }
    (*num_of_tokens) = (*num_of_tokens) * 2;
    return str;
}

char **create_pointers(char *str, int num_of_tokens) {
    char **pointers = (char**)malloc(sizeof(char*) * num_of_tokens);
    char *ptr;
    char delim[] = {' ',':','\n'};
    int i = 0;
    ptr = strtok(str, delim);
    while (ptr != NULL) {
        pointers[i++] = ptr;
        ptr = strtok(NULL, delim);
    }
    return pointers;
}

slave *create_slaves(char **pointers, int num_of_tokens) {
    slave *worker = (slave*)malloc(sizeof(slave));
    worker->amount = num_of_tokens / 2;
    worker->ptr = (desc*)malloc(sizeof(desc) * num_of_tokens / 2);
    int i = 0, j = 0;
    while(i < num_of_tokens - 1) {
        worker->ptr[j].name = pointers[i];
        i++;
        worker->ptr[j].time = atoi(pointers[i]);
        i++;
        j++;

    }
    return worker;
}

slave *create_worker(const char *file_name) {
    char **pointers;
    int num_of_tokens;
    char *str = input_information(file_name, &num_of_tokens);
    pointers = create_pointers(str, num_of_tokens);
    slave *worker = create_slaves(pointers, num_of_tokens);
    //for (int i = 0; i < worker->amount; i++) {\
        printf("%s %d\n", worker->ptr[i].name, worker->ptr[i].time);\
    }
    return worker;
}

dish *create_dish(const char *file_name) {
    char **pointers;
    int num_of_tokens;
    char *str = input_information(file_name, &num_of_tokens);
    pointers = create_pointers(str, num_of_tokens);
    dish *list = (dish*)malloc(sizeof(dish) * num_of_tokens / 2);
    AMOUNT = num_of_tokens / 2;
    int i = 0, j = 0;
    while(i < num_of_tokens - 1) {
        list[j].name = pointers[i];
        i++;
        list[j].num = atoi(pointers[i]);
        i++;
        j++;

    }
    //for (i = 0; i < AMOUNT; i++) {\
        printf("%s %d\n", list[i].name, list[i].num);\
    }
    return list;
}
   
void my_semop(int semid, int semnum, int n){
    struct sembuf result;
    result.sem_op = n;
    result.sem_flg = 0;
    result.sem_num = semnum;
    semop(semid, &result, 1);
}   


void *my_slave1(void *dummy)
{
    int fd;
    slave *worker = (slave*)(dummy);
    fd = open("table.txt", O_RDWR);
    for (int i = 0; i < AMOUNT; i++) {
        for (int k = 0; k < list[i].num; k++){
            for (int j = 0; j < worker->amount; j++) {
                if (strcmp(list[i].name, worker->ptr[j].name) == 0) {
                    printf("Я раб номер ОДИН, МОЮ предмет %s\n", list[i].name);
                    sleep(worker->ptr[j].time);
                    my_semop(semid, 0, -1);
                    write(fd, list[i].name, strlen(list[i].name));
                    write(fd, "\n", 1);
                    my_semop(semid, 1, 1);
                    printf("Я раб номер ОДИН, ПОМЫЛ предмет %s и положил на стол\n", list[i].name); 
                    break;
                }
            }
        }
    }
    write(fd, "RUN", 3);
    write(fd, "\n", 1);
    close(fd);
    my_semop(semid, 1, 1);
    printf("Я раб номер ОДИН, ВСЕ ПОМЫЛ\n\tЯ РАБ НОМЕР ОДИН СВОБОДЕН\nГОВОРЮ ВТОРОМУ РАБУ RUN!\n");
    return NULL;
}

void *my_slave2(void *dummy)
{
    slave *worker = (slave*)(dummy);
    char *buf = (char*)calloc(BIG_CONST, sizeof(char));
    int i;
    int fd = open("table.txt", O_RDONLY); 
    while (1){
        i = 0;
        my_semop(semid, 1, -1);
        read(fd, buf, 1);
        while (buf[i] != '\n') {
        i++;
        read(fd, buf + i, 1);
        }
        buf[i] = '\0';
        my_semop(semid, 0, 1);
        printf("Я раб номер ДВА, ВЗЯЛ со стола предмет %s\n", buf);
        for (int j = 0; j < worker->amount; j++) {
            if (strcmp(buf, worker->ptr[j].name) == 0) {
                printf("Я раб номер ДВА, ТРУ предмет %s\n", worker->ptr[j].name);
                sleep(worker->ptr[j].time);
                printf("Я раб номер ДВА, ПРОТЕР предмет %s\n", worker->ptr[j].name); 
                break;
            }
        }
        if (strcmp(buf, "RUN") == 0) {
            printf("\tЯ РАБ НОМЕР ДВА ВСЕ ПРОТЕР, И ТЕПЕРЬ Я СВОБОДЕН\n");
            return NULL;
        }
    }
    close(fd);
    return NULL;
}

int main() {
    slave *worker1 = create_worker("worker1.txt");
    slave *worker2 = create_worker("worker2.txt");
    list = create_dish("dish.txt");
    key_t key;
    key = ftok("restraunt.c", 0);
    semid = semget(key , 2 , 0666 | IPC_CREAT);
    pthread_t mthid_1, mthid_2;
    my_semop(semid, 0, TABLE_LIMIT);
    int fd;
    if ((fd = open("table.txt", O_CREAT | O_EXCL, 0666)) < 0) {
        fd = open("table.txt", O_TRUNC, 0666);
        close(fd);
    }
    pthread_create(&mthid_1, (pthread_attr_t *)NULL, my_slave1, (void *)(worker1));
    pthread_create(&mthid_2, (pthread_attr_t *)NULL, my_slave2, (void *)(worker2));
    pthread_join(mthid_1, (void **)NULL);
    pthread_join(mthid_2, (void **)NULL);
    semctl(semid, 0, IPC_RMID, 0);
    semctl(semid, 1, IPC_RMID, 0);
    return 0;
}
