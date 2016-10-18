#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int semid;
    char buf[] = "НО Я ИХ ЗАБОТАЛ!!!";
    const char pathname[] = "kek.out";
    key_t key;
    struct sembuf mybuf;
    key = ftok(pathname , 0);
    semid = semget(key , 1 , 0666 | IPC_CREAT);
    mybuf.sem_op = -2;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;
    int i = 20;
    FILE *file;
    while (i > 0) {
        semop(semid , &mybuf , 1);
        file = fopen("kek.out", "a+");
        fprintf(file,"%s \n", buf);
        fclose(file);
        semop(semid , &mybuf , 1);
        i--;
        }
    semctl(semid, IPC_RMID, 0);
    return 0;
}
