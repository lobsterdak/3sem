#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 

char messeage[] = "LUKE, I AM YOUR FATHER!";

#define send_bit_father SIGUSR1
#define read_0_bit_son SIGUSR1
#define read_1_bit_son SIGUSR2

pid_t son_pid;
pid_t father_pid;

int messeage_length;
int number_of_symbol;

int send_bit_counter;
char sending_symbol;

int get_bit_counter;
char getting_symbol;

void send_bit(int nsig){
    if (send_bit_counter == 8){
        number_of_symbol++;
        if (number_of_symbol == messeage_length){
            kill(son_pid, SIGKILL);
            printf("\n\tNOOOOOOOOOOOOOO");
            exit(0);
        }
        sending_symbol = messeage[number_of_symbol];
        send_bit_counter = 0;
    }
    if ((sending_symbol >> (7 - send_bit_counter)) & 1){
        kill(son_pid, read_1_bit_son);
        send_bit_counter++;
    }
    else{
        kill(son_pid, read_0_bit_son);
        send_bit_counter++;
    }
}

void get_0_bit(int nsig){
    getting_symbol = ((getting_symbol << 1) | 0);
    get_bit_counter++;
    if (get_bit_counter == 8){
        fprintf(stderr,"%c", getting_symbol); //не знаю почему, но если выводить в обычный поток вывода без \n на конце, то вовсе ничего не выводит
        get_bit_counter = 0;
        sleep(1);
    }
    kill(father_pid, send_bit_father);
}

void get_1_bit(int nsig){
    getting_symbol = ((getting_symbol << 1) | 1);
    get_bit_counter++;
    if (get_bit_counter == 8){
        fprintf(stderr,"%c", getting_symbol); //не знаю почему, но если выводить в обычный поток вывода без \n на конце, то вовсе ничего не выводит
        get_bit_counter = 0;
        sleep(1);
    }
    kill(father_pid, send_bit_father);
}

int main(){
    number_of_symbol = 0;
    send_bit_counter = 0;
    get_bit_counter = 0;
    messeage_length = strlen(messeage) + 1;
    sending_symbol = messeage[0];
    son_pid = fork();
        if (son_pid == 0){
            signal(read_0_bit_son, get_0_bit);
            signal(read_1_bit_son, get_1_bit);
            father_pid = getppid();
            sleep(1);
            kill(father_pid, send_bit_father);
            while (1) ;
        }
        else {
            signal(send_bit_father, send_bit);
            while (1) ;
        }
    return 0;
}

