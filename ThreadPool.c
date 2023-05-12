#include <stdlib.h>
#include <pthread.h>
#include "ThreadPool.h"

// https://nachtimwald.com/2019/04/12/thread-pool-in-c/

struct taskQueue {
    threadFunc func;
    void *arg; // This is a node in the LinkedList
    struct taskQueue *nextTask; // Pointer to next task (or NULL)
};
typedef struct taskQueue taskQueue_t;

struct tpool {
    taskQueue_t *firstTask;
    taskQueue_t *lastTask;
    pthread_mutex_t taskMutex; // A single mutex used for all locking
    pthread_cond_t waitForTasks; // Signals when there are new tasks to proces
    pthread_cond_t finishedTasks; // Signals when there are no threads processing
    size_t numActiveThreads; // Counts the number of threads that are processing
    size_t numThreads; // Number of threads ALIVE.
    int stop; // Flag to stop threads
};

//*** HELPER FUNCTIONS ***//
taskQueue_t *createNewTask(threadFunc func, void *arg) {
    taskQueue_t *newTask;
    if (func == NULL) { return NULL; }
    newTask = malloc(sizeof(*newTask));
    newTask->func = func;
    newTask->arg = arg;
    newTask->nextTask = NULL;
    return newTask;
}

void freeTask(taskQueue_t *task) {
    if (task != NULL) { free(task); }
}

/**
 * This function removes and returns the next task in the queue.
 */
taskQueue_t *getTask(tpool_t *pool) {
    taskQueue_t *task;
    if (pool == NULL) { return NULL; }
    task = pool->firstTask;
    if (task == NULL) { return NULL; }
    if (task->nextTask == NULL) { // This is the last task!
        pool->firstTask = NULL;
        pool->lastTask = NULL;
    } else { // Remove task and continue to next node
        pool->firstTask = task->nextTask;
    }
    return task;
}

/**
 * This is the function that the threads in the pool will run.
 * @param arg The thread pool
 */
void *tpoolTaskHandler(void *arg) {
    tpool_t *pool = arg;
    taskQueue_t *task;
    while (1) { // As long as the thread does not exit - keep running!

        pthread_mutex_lock(&(pool->taskMutex));
        while (pool->firstTask == NULL && !pool->stop) { // USE WHILE INSTEAD OF IF IN CASE OF SPURIOUS WAKE-UPS
            // Unlocks the mutex, the lock is reacquired when signaled
            pthread_cond_wait(&(pool->waitForTasks), &(pool->taskMutex));
        }
        if (pool->stop) { break; }
        task = getTask(pool); // Returns next task in the thread pool
        pool->numActiveThreads++;
        pthread_mutex_unlock(&(pool->taskMutex));

        if (task != NULL) { // Make sure that getTask returned a task
            task->func(task->arg);
            freeTask(task);
        }

        pthread_mutex_lock(&(pool->taskMutex));
        // The lock ensures that numActiveThreads is zero when the last thread finishes its task
        pool->numActiveThreads--;
        if (!pool->stop && pool->numActiveThreads == 0 && pool->firstTask == NULL) {
            pthread_cond_signal(&(pool->finishedTasks));
        }
        pthread_mutex_unlock(&(pool->taskMutex));
    }
    // Reaches here when pool->stop == TRUE
    pool->numThreads--;
    pthread_cond_signal(&(pool->finishedTasks)); // Signals that a thread has exited
    pthread_mutex_unlock(&(pool->taskMutex));
    return NULL;
}

//***** PUBLIC FUNCTIONS DECLARED IN THE HEADER *****//

tpool_t *tpoolCreate(size_t num) {
    tpool_t *pool;
    pthread_t thread;
    size_t i;
    if (num == 0) { num = 1; }
    pool = calloc(1, sizeof(*pool));
    pool->numThreads = num;
    pthread_mutex_init(&(pool->taskMutex), NULL);
    pthread_cond_init(&(pool->waitForTasks), NULL);
    pthread_cond_init(&(pool->finishedTasks), NULL);
    pool->firstTask = NULL;
    pool->lastTask = NULL;
    pool->stop = 0;

    for (i = 0; i < num; ++i) {
        pthread_create(&thread, NULL, tpoolTaskHandler, pool);
        pthread_detach(thread); // Threads run independently, no need for join (they will clean up on exit).
    }
    return pool;
}

void tpoolDestroy(tpool_t *pool) {
    taskQueue_t *task;
    taskQueue_t *tmp;
    if (pool == NULL) { return; }

    pthread_mutex_lock(&(pool->taskMutex));
    task = pool->firstTask;
    // Empty the queue
    // The mutex ensures that threads which are processing have already removed their tasks
    while (task != NULL) {
        tmp = task->nextTask;
        freeTask(task);
        task = tmp;
    }
    pool->stop = 1;
    pthread_cond_broadcast(&(pool->waitForTasks));
    pthread_mutex_unlock(&(pool->taskMutex));

    tpoolWait(pool);
    pthread_mutex_destroy(&(pool->taskMutex));
    pthread_cond_destroy(&(pool->waitForTasks));
    pthread_cond_destroy(&(pool->finishedTasks));
    free(pool);
}

/**
 * This function creates a new tasks and adds it to the queue.
 */
int tpoolAddTask(tpool_t *pool, threadFunc func, void *arg) {
    taskQueue_t *task;
    if (pool == NULL) { return 0; }
    task = createNewTask(func, arg);
    if (task == NULL) { return 0; }

    pthread_mutex_lock(&(pool->taskMutex));
    if (pool->firstTask == NULL) {
        pool->firstTask = task;
        pool->lastTask = pool->firstTask;
    } else {
        pool->lastTask->nextTask = task;
        pool->lastTask = task;
    }
    pthread_cond_broadcast(&(pool->waitForTasks));
    pthread_mutex_unlock(&(pool->taskMutex));

    return 1;
}

/**
 * Blocking function. Waits if:
 * 1. There still are threads processing
 * 2. There are more tasks (wait for pool->finishedTasks to be signaled)
 * 3. The stop parameter is true, but not all threads have exited
 * @param pool
 */
void tpoolWait(tpool_t *pool) {
    if (pool == NULL) { return; }

    pthread_mutex_lock(&(pool->taskMutex));
    while (1) {
        if ((!pool->stop && pool->numActiveThreads != 0) ||
            (pool->stop && pool->numThreads != 0)) {
            pthread_cond_wait(&(pool->finishedTasks), &(pool->taskMutex));
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&(pool->taskMutex));
}