#ifndef THREADPOOL_MANAGER_LINKEDLIST_H
#define THREADPOOL_MANAGER_LINKEDLIST_H


struct node {
    char *data;
    struct node *next;
    int key;
    int status; // 1 if processed, otherwise 0
};

struct node *createNode(char *data, int key);

struct node* insertNode(struct node **head, char *data, int key);

void *printList_t(void *arg);

void freeList(struct node *head);

#endif //THREADPOOL_MANAGER_LINKEDLIST_H
