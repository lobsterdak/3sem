#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define LAST_MESSAGE 255 

int main() {
    int msqid; 
    char pathname[] = "exercise2_1.c"; 
    key_t key; 
    int maxlen = 81, len; 
    struct mymsgbuf {
        long mtype;
        struct {
            int x, y, z;
            } info;
    } mybuf;
    key = ftok(pathname, 0);
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    while (1) {
        mybuf.mtype = 1;
        scanf("%d %d", &mybuf.info.x, &mybuf.info.y);
        if (msgsnd(msqid, (struct msgbuf *)&mybuf, sizeof(mybuf.info), 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }
        if ((msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 2, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        printf("%d * %d = %d\n",mybuf.info.x, mybuf.info.y, mybuf.info.z);
    }
    return 0;
} 
