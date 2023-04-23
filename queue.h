#ifndef Queue_H



struct Node
{
    void* data;
    struct Node* next;
};

typedef struct Node Node;


struct Queue
{
    void* data;
    struct Node* head;
    struct Node* tail;
    int size;
};


typedef struct Queue Queue;

Queue* Qcreate();

void destroy(Queue* q);

void enqueue(Queue* q, void* data);

void* dequeue(Queue* q);

int is_empty(Queue* q);

int queue_size(Queue* q);

#endif