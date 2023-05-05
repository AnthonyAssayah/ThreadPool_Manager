#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
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
    //*** VALIDATE ARGUMENTS ***//
    if (argc != 3) {
        fprintf(stderr, "Invalid input!\n"
                        "Usage:\n"
                        "(1) ./coder key -e/-d < input_file > output_file\n"
                        "(2) cat input_file | ./coder 2 -e/-d > output_file\n");
        return 2;
    }
    if (strcmp(argv[2], "-e") == 0) {
        codecFunc = encrypt;
    } else if (strcmp(argv[2], "-d") == 0) {
        codecFunc = decrypt;
    } else {
        fprintf(stderr, "You must choose to encrypt or decrypt!\n"
                        "Valid flags: -e or -d\n");
        return 3;
    }
    int key = atoi(argv[1]); // Get encryption / decryption key

    //*** HANDLE ENCRYPTION / DECRYPTION ***//
    char c;
    int cnt = 0;
    char data[MAX_SIZE + 1];
    memset(data, 0, MAX_SIZE + 1);
    struct node *head = NULL; // Linked List to dynamically save data!
    struct node *new_node = NULL;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    time_t begin = time(NULL);
    int n_threads = get_nprocs(); // Maximum number of threads available
//    int n_threads = 1;
    tpool_t *pool = tpoolCreate(n_threads);

    while ((c = getchar()) != EOF) {
        data[cnt++] = c;
        if (cnt == MAX_SIZE) {
            data[cnt] = '\0';
            new_node = insertNode(&head, data, key);
            tpoolAddTask(pool, worker, new_node);
            cnt = 0;
        }
    }
    // Last data before EOF was reached
    if (cnt > 0) {
        data[cnt] = '\0';
        new_node = insertNode(&head, data, key);
        tpoolAddTask(pool, worker, new_node);
        sleep(1);
    }

    tpoolWait(pool);
    tpoolDestroy(pool);

    printList(head);
    freeList(head);

    // calculate elapsed time by finding difference (end - begin)
    time_t end = time(NULL);
    fprintf(stderr, "The elapsed time is %ld seconds\n", (end - begin));
    return 0;
}