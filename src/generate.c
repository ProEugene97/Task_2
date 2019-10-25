#include "generate.h"

void generate_array(char *const array, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        array[i] = rand() % (122 - 48 + 1) + 48;
    }
}