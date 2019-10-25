#include "../include/count.h"

int count(const char *const a, int n) {
    int capacity = 40;
    int size = 0;
    data *array = (data *) malloc(capacity * sizeof(data));
    char c = a[0];
    int length = 1, ind = 0, i = 0;
    do {
        ++i;
        if (i < n && a[i] == c) {
            ++length;
        } else {
            for (; ind < size && length != array[ind].length; ++ind);
            if (ind == size) {
                array[ind].length = length;
                array[ind].quantity = 0;
                ++size;
            }
            ++array[ind].quantity;
            array[ind].c = c;
            length = 1;
            if (i < n)
                c = a[i];
            ind = 0;
            if (size == capacity) {
                capacity *= 2;
                data *tmp = (data *)realloc(array, capacity * sizeof(data));
                if (tmp != NULL)
                    array = tmp;
                else {
                    free (array);
                    exit (1);
                }
            }
        }
    } while (i < n);
    length = array[0].length;
    int quant = array[0].quantity;
    //c = array[0].c;
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
    free(array);
    return quant;
}