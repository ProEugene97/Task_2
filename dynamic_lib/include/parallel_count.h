#ifndef TASK_2_PARALLEL_COUNT_H
#define TASK_2_PARALLEL_COUNT_H

#include "data.h"
#include <pthread.h>
#include <stdlib.h>

#define THREADS 4

typedef struct{
    const char *a;
    data *array;
    int first;
    int last;
    int len;
    int *size;
    int capacity;
    pthread_mutex_t *mutex;
} pthrData;

int parallel_count(const char *const a, int n);

#endif //TASK_2_PARALLEL_COUNT_H
