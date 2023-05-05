#ifndef THREADPOOL_MANAGER_THREADPOOL_H
#define THREADPOOL_MANAGER_THREADPOOL_H

#include <stddef.h>

struct tpool;
typedef struct tpool tpool_t;

typedef void (*threadFunc)(void *arg);

tpool_t *tpoolCreate(size_t num);

void tpoolDestroy(tpool_t *pool);

int tpoolAddTask(tpool_t *pool, threadFunc func, void *arg);

void tpoolWait(tpool_t *pool);

#endif //THREADPOOL_MANAGER_THREADPOOL_H
