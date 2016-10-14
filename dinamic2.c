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
    char *str;
    int shmid;
    char pathname[] = "06-1a.txt";
    key_t key;
    key = ftok(pathname, 0);
    shmid = shmget (key, 1000 * sizeof(char), 0);
    str = (char *)shmat(shmid, NULL, 0);
    printf("УРА УРА \n%s", str);
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
