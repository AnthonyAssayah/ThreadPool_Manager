#include <string.h>
#include <stdio.h>
#include "LinkedList.h"
#include "stdlib.h"

/**
 * Function to create a new node and initialize its data and next pointers.
 */
struct node *createNode(char *data, int key) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    new_node->data = strdup(data);
    new_node->key = key;
    new_node->next = NULL;
    new_node->status = 0;
    return new_node;
}

/**
 * Function to insert a new node at the end of the linked list.
 */
struct node *insertNode(struct node **head, char *data, int key) {
    struct node *new_node = createNode(data, key);
    if (*head == NULL) {
        *head = new_node;
    } else {
        struct node *tmp = *head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new_node; // Reached the end of the list!
    }
    return new_node;
}

/**
 * Thread safe function to print the contents of the linked list.
 */
void *printList_t(void *arg) {
    struct node *tmp = arg; // arg = the head of the LinkedList
    while (tmp != NULL) {
        while (tmp->status == 0); // busy wait
        printf("%s", tmp->data);
        tmp = tmp->next;
    }
    return NULL;
}

/**
 * Function to free the memory allocated to the linked list nodes
 */
void freeList(struct node *head) {
    struct node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->data);
        free(tmp);
    }
}