#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

int msqid;
struct mymsgbuf {
        long mtype;
        struct {
            int x, y, z;
            } info;
    } mybuf;

void *mythread(void *dummy) {
    printf("start %d * %d\n", mybuf.info.x, mybuf.info.y);
    mybuf.info.z = mybuf.info.x * mybuf.info.y;
    mybuf.mtype = 2;
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0)
    {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
      exit(-1);
    }
    printf("finish\n");
    return NULL;
}

int main() {
    char pathname[] = "exercise2_1.c"; 
    key_t key;
    pthread_t mythid; 
    int len, maxlen; 
    key = ftok(pathname, 0);
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    while (1) {
        maxlen = 81;
        if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        pthread_create(&mythid, (pthread_attr_t *) NULL, mythread, NULL);
        pthread_join(mythid, (void **)NULL);
    }
    return 0;
} 
