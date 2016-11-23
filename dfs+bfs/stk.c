#include "stk.h"
#include <stdlib.h>
#include <stdio.h>

int is_empty(struct Stack *stk){
    return (stk->pointer == 0) ? 1 : 0 ; 
}

struct Stack *create_stk(){
    struct Stack *stk = (struct Stack*)calloc(1, sizeof(struct Stack));
    stk->pointer = 0;
    return stk;
} 

Data pop(struct Stack *stk){
    if (stk->pointer == 0){
        fprintf(stderr, "EMPTY STACK\n");
        return EMPTY_STACK;
    }
    else {
        stk->pointer --; 
        return stk->array[stk->pointer];
    }
}

Data top(struct Stack *stk){
    if (stk->pointer == 0){
        fprintf(stderr, "EMPTY STACK\n");
        return EMPTY_STACK;
    }
    else {
        return stk->array[stk->pointer-1];
    }
} 
 

void push (struct Stack *stk, Data data){
    if (stk->pointer == MAX_STACK_SIZE)
        fprintf(stderr, "STACK OVERFLOW\n");
    else {
        stk->array[stk->pointer] = data;
        stk->pointer++;
    }
    return;
}

void print(struct Stack *stk){
    for (int i = 0; i < stk->pointer; i++){
        printf("%d\n", pop(stk));
    }
}
