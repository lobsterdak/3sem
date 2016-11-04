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
    char pathname[] = "exercise1_1.c"; 
    key_t key; 
    int i, len; 
    struct mymsgbuf {
        long mtype;
        struct {
            int number;
            } info;
    } mybuf;
    key = ftok(pathname, 0);
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    for (i = 1; i < 6; i++) {
        mybuf.mtype = 1;
        mybuf.info.number = i * i;
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }
    }
    mybuf.mtype = LAST_MESSAGE;
    len = 0;
    if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    return 0;
} 
