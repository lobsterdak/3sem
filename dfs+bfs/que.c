#include <stdio.h>
#include <stdlib.h>
#include "que.h"

struct Queue *create_queue(){
    struct Queue *que = (struct Queue *)malloc(sizeof(struct Queue));
    que->head = MAX_QUEUE_SIZE / 2 - 1;
    que->tail = MAX_QUEUE_SIZE / 2;
    return que;
}

void push_q_back(struct Queue *que, Data element){
    if (que->tail == MAX_QUEUE_SIZE - 1){
        fprintf(stderr, "QUEUE BACK IS FULL\n");
        return ;
    }
    else {
        que->array[que->tail] = element;
        que->tail++ ;
    }
}

void push_q_front(struct Queue *que, Data element){
    if (que->head == 0){
        fprintf(stderr, "QUEUE FRONT IS FULL\n");
        return ;
    }
    else {
        que->array[que->head] = element;
        que->head-- ;
    }
}

Data pop_q_back(struct Queue *que){
    if (que->tail == que->head + 1){
        fprintf(stderr, "QUEUE IS EMPTY\n");
    }
    que->tail --;
    return que->array[que->tail];
}

Data pop_q_front(struct Queue *que){
    if (que->tail == que->head + 1){
        fprintf(stderr, "QUEUE IS EMPTY\n");
    }
    que->head ++;
    return que->array[que->head];
}

Data top_q_front(struct Queue *que){
    if (que->tail == que->head + 1){
        fprintf(stderr, "QUEUE IS EMPTY\n");
    }
    return que->array[que->head + 1];
}

int is_empty_q(struct Queue *que){
    return (que->tail == que->head + 1) ? 1 : 0;
}
