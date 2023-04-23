
/* queue implementation*/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new queue
Queue *Qcreate()
{
    Queue *q = malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

// Destroy a queue
void destroy(Queue *q)
{
    if (q == NULL)
    {
        return;
    }
    while (q->head != NULL)
    {
        Node *temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
    free(q);
}

// Add an element to the queue
void enqueue(Queue *q, void *p)
{
    if (q == NULL)
    {
        return;
    }
    Node *newNode = malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->data = p;
    if (q->head == NULL)
    {
        q->head = newNode;
        q->tail = newNode;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
}

// Remove the first element from the queue
void* dequeue(Queue *q)
{
    if (q == NULL)
    {
        return NULL;
    }
    if (q->head == NULL)
    {
        return NULL;
    }
    Node *temp = q->head;
    q->head = q->head->next;
    void *data = temp->data;
    free(temp);
    q->size--;
    return data;
}

// Return the first element of the queue
char *queue_first(Queue *q)
{
    if (q == NULL)
    {
        return NULL;
    }
    if (q->head == NULL)
    {
        return NULL;
    }
    return q->head->data;
}


// Return the last element of the queue
char *queue_last(Queue *q)
{
    if (q == NULL)
    {
        return NULL;
    }
    if (q->tail == NULL)
    {
        return NULL;
    }
    return q->tail->data;
}


// Return the number of elements in the queue
int queue_size(Queue *q)
{
    if (q == NULL)
    {
        return 0;
    }
    return q->size;
}

int is_empty(Queue *q)
{
    if (q == NULL)
    {
        return -1;
    }
    return q->size == 0;
}



int main(int argc, char *argv[])
{
    Queue *q = Qcreate();
    enqueue(q, "hello");
    enqueue(q, "world");
    enqueue(q, "!");


// print enqueued data
    printf("first: %s \n", queue_first(q));
    printf("last: %s \n", queue_last(q));
    dequeue(q);
    printf("first: %s \n", queue_first(q));
    printf("last: %s \n", queue_last(q));
    dequeue(q);
    printf("size: %i \n", queue_size(q));
    printf("is empty: %i \n", is_empty(q));
    printf("first: %s \n", queue_first(q));
    printf("last: %s \n", queue_last(q));
    dequeue(q);
    printf("size: %i \n", queue_size(q));
    printf("is empty: %i \n", is_empty(q));

    destroy(q);
}