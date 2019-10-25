#include <stdio.h>


#include <stdlib.h>
#include "../static_lib/include/count.h"
#include <time.h>
#include "generate.h"
#include "../dynamic_lib/include/parallel_count.h"


int main(int argv, char **argc) {
    char *a = (char *)malloc(SIZE * sizeof(char));
    generate_array(a, SIZE);
    time_t t0 = time(0);
    int c1 = parallel_count(a, SIZE);
    time_t t1 = time(0);
    double time_in_seconds1 = difftime(t1, t0);
    time_t t2 = time(0);
    int c2 = count(a, SIZE);
    time_t t3 = time(0);
    double time_in_seconds2 = difftime(t3, t2);
    printf("parallel: %f  consistently: %f\n", time_in_seconds1, time_in_seconds2);
    free(a);
    return 0;
}