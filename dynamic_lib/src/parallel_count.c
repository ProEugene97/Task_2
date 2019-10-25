#include "../include/parallel_count.h"

void * one_thread_count(void *param) {
    pthrData* threadData = (pthrData*) param;
    char c = threadData->a[threadData->first];
    if (threadData->first != 0 && threadData->a[threadData->first - 1] == c)
        while (threadData->first + 1 < threadData->last && threadData->a[++threadData->first] == c);
    if (threadData->first + 1 == threadData->last)
        pthread_exit(NULL);
    c = threadData->a[threadData->first];
    int length = 1, ind = 0, i = threadData->first;
    do {
        ++i;
        if (i < threadData->last && threadData->a[i] == c)
            ++length;
        else {
            for (; ind < *threadData->size && length != (*threadData->array)[ind].length; ++ind);
            if (ind == *threadData->size) {
                (*threadData->array)[ind].length = length;
                (*threadData->array)[ind].quantity = 0;
                ++*threadData->size;
            }
            ++(*threadData->array)[ind].quantity;
            (*threadData->array)[ind].c = c;
            if (*threadData->size == threadData->capacity) {
                threadData->capacity *= 2;
                data *tmp = (data *)realloc(*threadData->array, threadData->capacity * sizeof(data));
                if (tmp != NULL)
                    *threadData->array = tmp;
                else {
                    free (*threadData->array);
                    exit (1);
                }
            }
            if (i < threadData->last)
                c = threadData->a[i];
            length = 1;
            ind = 0;
        }
        if (i + 1 == threadData->last && threadData->last != threadData->len && threadData->a[threadData->last] == c)
            ++threadData->last;
    } while (i < threadData->last);
    pthread_exit(NULL);
}

int parallel_count(const char *const a, int n) {
    int cap = THREADS * 10;
    data* arrays[THREADS];
    int sizes[THREADS];
    pthrData threadData[THREADS];
    pthread_t tid[THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    for (int i = 0; i < THREADS; i++) {
        sizes[i] = 0;
        arrays[i] = (data *)malloc((cap / THREADS) * sizeof(data));
        threadData[i].a = a;
        threadData[i].array = &arrays[i];
        threadData[i].first = i * n / THREADS;
        threadData[i].last = threadData[i].first + n / THREADS;
        threadData[i].len = n;
        threadData[i].size = &sizes[i];
        threadData[i].capacity = cap / THREADS;
        threadData[i].mutex = &mutex;
        pthread_create(&tid[i], NULL, one_thread_count, (void *)&threadData[i]);
    }
    for (int i = 0; i < THREADS; i++) pthread_join(tid[i], NULL);
    int size = 0;
    for (int i = 0; i < THREADS; ++i)
        size += sizes[i];
    data *array = (data *)calloc(size, sizeof(data));
    for (int i = 0; i < sizes[0]; ++i) {
        array[i] = arrays[0][i];
    }
    for (int i = 1; i < THREADS; ++i) {
        for (int j = 0; j < sizes[i]; ++j)
            for (int k = 0; k < size; ++k) {
                if (array[k].length == arrays[i][j].length) {
                    array[k].quantity += arrays[i][j].quantity;
                    break;
                }
                if (array[k].length == 0) {
                    array[k].length = arrays[i][j].length;
                    array[k].quantity = arrays[i][j].quantity;
                    array[k].c = arrays[i][j].c;
                    break;
                }
            }
    }
    int length = array[0].length;
    int quant = array[0].quantity;
    //char c = array[0].c;
    for (int j = 1; j < size; ++j) {
        if (array[j].quantity > quant) {
            length = array[j].length;
            quant = array[j].quantity;
            //c = array[j].c;
        } else if (array[j].quantity == quant && array[j].length > length) {
            length = array[j].length;
            quant = array[j].quantity;
            //c = array[j].c;
        }
    }
    for (int i = 0; i < THREADS; ++i)
        free(arrays[i]);
    free(array);
    return quant;
}