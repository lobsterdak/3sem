#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {
    int file;
    char * str;
    char string[1000];
    int shmid;
    char pathname[] = "06-1a.txt";
    key_t key;
    key = ftok(pathname, 0);
    shmid = shmget (key, 1000 * sizeof(char), 0666|IPC_CREAT);
    str = (char *)shmat(shmid, NULL, 0);
    file = open("dinamic1.c", O_RDONLY);
    while(read(file, string, 1000) > 0) {
        strcat(str, string);
    }
    close(file);
    shmdt(str);
    return 0;
}
