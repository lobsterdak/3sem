#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stk.h"
#include "que.h"

int **read_from_file(const char *name, int *num_of_tops);
void print_array_tree(int **tree, int num_of_tops);
void dfs(int **tree, int *used, int num_of_tops, int top, struct Stack *stk);
void bfs(int **tree, int *used, int num_of_tops, int start, struct Queue *que);
 
int main (int argc, char **argv){
    if (argc != 3){
        printf("To few or many arguments\n");
        return 0;
    }
    int num_of_tops = 0;
    int **tree = read_from_file(argv[1], &num_of_tops);
    int *used = (int*)calloc(num_of_tops, sizeof(int));
    print_array_tree(tree, num_of_tops);
    if (strcmp(argv[2], "dfs") == 0){
        struct Stack *stk = create_stk();
        dfs(tree, used, num_of_tops, 0, stk);
    }
    else { 
        if (strcmp(argv[2], "bfs") == 0){
            struct Queue * que = create_queue();
            bfs(tree, used, num_of_tops, 0, que);
        }
        else 
            printf("BAD ARGUMENTS\n");
    }
    //for (int h = 0; h < num_of_tops; h++)\
        printf(" %d %d\n", h, used[h]); 
    return 0;
}

void bfs(int **tree, int *used, int num_of_tops, int start, struct Queue *que){
    int i = 0;
    push_q_back(que, start);
    used[start] = 1;
    while (is_empty_q(que) != 1){
        //printf("ЖОРА %d\n", is_empty_q(que));
        for (i = 0; i < num_of_tops; i++){
            //printf("%d #%d\n", top_q_front(que), i);\
            printf("\tСМЕЖНОСТЬ %d \n \tЮЗАБЕЛЬНОСТЬ %d\n", tree[top_q_front(que)][i], used[i]); 
            if ((tree[top_q_front(que)][i]) && (used[i] == 0)){
                push_q_back(que, i);
                used[i] = 1; 
            }
        }
    printf("ВЕРШИНА %d\n", pop_q_front(que));
    }
}

void dfs(int **tree, int *used, int num_of_tops, int start, struct Stack *stk){
    int i = 0;
    push(stk, start);
    //printf("Веришна %d\n", start);
    used[start] = 1;
    while (is_empty(stk) != 1){
        printf("Веришна %d\n", top(stk));
        for (i = 0; i < num_of_tops; i++){
            if (is_empty(stk) == 1)
                return ;
            if ((tree[top(stk)][i]) && (used[i] == 0))
                dfs(tree, used, num_of_tops, i, stk); 
        }
        if (is_empty(stk) != 1)
            pop(stk);
    }
}

int **read_from_file(const char *name, int *num_of_tops){
int j = 0, i = 0;
FILE *file = fopen(name, "r");
fscanf(file, "%d", num_of_tops);
int **tree = (int **)malloc(sizeof(int*) * (*num_of_tops));
for (j = 0; j < (*num_of_tops); j++){
    tree[j] = (int *)malloc(sizeof(int) * (*num_of_tops)); 
    for (i = 0; i < (*num_of_tops); i++){
        fscanf(file, "%d", tree[j] + i);
    }
}
fclose(file);
return tree;
}

void print_array_tree(int **tree, int num_of_tops){
int j, i;
for (j = 0; j < num_of_tops; j++){
        for (i = 0; i < num_of_tops; i++){
            printf("%d  ", tree[j][i]);
        }
        printf("\n");
    }
}
