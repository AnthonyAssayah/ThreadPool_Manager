#ifndef THREADPOOL_MANAGER_LINKEDLIST_H
#define THREADPOOL_MANAGER_LINKEDLIST_H


struct node {
    char *data;
    struct node *next;
    int key;
};

struct node *createNode(char *data, int key);

struct node* insertNode(struct node **head, char *data, int key);

void printList(struct node *head);

void freeList(struct node *head);

#endif //THREADPOOL_MANAGER_LINKEDLIST_H
