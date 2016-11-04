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
    int len, maxlen; 
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
    while (1) {
        maxlen = 81;
        if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0)) < 0) {
          printf("Can\'t receive message from queue\n");
          exit(-1);
        }
        if (mybuf.mtype == LAST_MESSAGE) {
          msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
          exit(0);
        }
        printf("message type = %ld, info = %d\n", mybuf.mtype, mybuf.info.number);
    }

    return 0;
} 
