typedef int Data;
#define MAX_QUEUE_SIZE 100

struct Queue {
    int head;
    int tail;
    Data array[MAX_QUEUE_SIZE];
};

struct Queue *create_queue();
void push_q_back(struct Queue *que, Data element);
void push_q_front(struct Queue *que, Data element);
Data pop_q_back(struct Queue *que);
Data pop_q_front(struct Queue *que);
Data top_q_front(struct Queue *que);
int is_empty_q(struct Queue *que);
