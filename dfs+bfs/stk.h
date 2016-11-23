#define MAX_STACK_SIZE 100

typedef int Data;

enum Errors {
EMPTY_STACK = -133,
};

struct Stack {
int pointer;
Data array[MAX_STACK_SIZE];
};

int is_empty(struct Stack *stk);
struct Stack *create_stk();
Data pop(struct Stack *stk);
void push(struct Stack *stk, Data data);
void print(struct Stack *stk);
Data top(struct Stack *stk);
