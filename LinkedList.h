#ifndef THREADPOOL_MANAGER_LINKEDLIST_H
#define THREADPOOL_MANAGER_LINKEDLIST_H


struct node {
    char *data;
    struct node *next;
};

struct node *createNode(char *data);

void insertNode(struct node **head, char *data);

void printList(struct node *head);

void freeList(struct node *head);

#endif //THREADPOOL_MANAGER_LINKEDLIST_H
