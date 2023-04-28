#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "LinkedList.h"
#include "ThreadPool.h"

#define MAX_SIZE 1024

void (*codecFunc)(char *s, int key) = NULL;

void worker(void *arg) {
    struct node *current = (struct node *) arg;
    int key = current->key;
    char *data = current->data;
    codecFunc(data, key);
}

int main(int argc, char *argv[]) {
    int cnt = 0;
    int key = 0;
    int enc = 0;
    int dec = 0;
    char c;
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-e") == 0) {
            enc = 1;
            codecFunc = encrypt;
        } else if (strcmp(argv[i], "-d") == 0) {
            dec = 1;
            codecFunc = decrypt;
        } else if (i == 1) {
            key = atoi(argv[1]);
        } else {
            printf("Invalid input\n");
            printf("coder key -d/-e < input_file > output_file\n");
            return 1;
        }
    }
    if (enc == 1 && dec == 1) {
        printf("You can't encrypt and decrypt at the same time\n");
        return 1;
    }
    if (enc == 0 && dec == 0) {
        printf("You must choose to encrypt or decrypt\n");
        return 1;
    }


    char data[MAX_SIZE + 1];
    memset(data, 0, MAX_SIZE + 1);
    struct node *head = NULL; // Linked List to dynamically save data!
    struct node *new_node = NULL;

    int n_threads = get_nprocs() + 1; // Maximum number of threads available
//    int n_threads = 1;
    tpool_t *tm = tpool_create(n_threads);

    while ((c = getchar()) != EOF) {
        data[cnt++] = c;
        if (cnt == MAX_SIZE) {
            data[cnt] = '\0';
            new_node = insertNode(&head, data, key);
            tpool_add_work(tm, worker, new_node);
            cnt = 0;
        }
    }
    // Last data before EOF was reached
    if (cnt > 0) {
        data[cnt] = '\0';
        new_node = insertNode(&head, data, key);
        tpool_add_work(tm, worker, new_node);
    }

    tpool_wait(tm);

    printList(head);
    freeList(head);
    tpool_destroy(tm);
    return 0;
}