#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <stdlib.h>


void translate(uint64_t number, float *f_number, char *unit);

#ifndef NPAD
#define NPAD 0
#endif

typedef struct _elt {
    struct _elt *next;
    long int    pad[NPAD];
} ll_element;


void
ll_sequence(ll_element **array, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++) {
        array[i]->next = i == n - 1 ? array[0] : array[i + 1];
        // printf("Element at index=%d (%p), Next at (%p)\n", i, array[i], array[i]->next);
    }
}

void
ll_shuffle(ll_element **array, size_t n)
{
    size_t i;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand48(usec);

    for (i = n - 1; i > 0; i--) {
        size_t j = (unsigned int) (drand48() * (i + 1));
        ll_element *t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}


ll_element *
ll_build(size_t n, int shuf)
{
    size_t i;
    ll_element **copy;
    ll_element *array;

    array = calloc(n, sizeof(ll_element));
    if (!array) { return NULL; }

    copy = calloc(n, sizeof(ll_element *));
    if (!copy) { return NULL; }

    for (i = 0; i < n; i++) {
        copy[i] = (ll_element *) (array + i);
        copy[i]->next = i == n - 1 ? array : (array + i + 1);
        // printf("Element at index=%d (%p), Next at (%p)\n", i, copy[i], copy[i]->next);
    }

    if (shuf) {
        ll_shuffle(copy, n);
    }

    ll_sequence(copy, n);
    return array;
}

void
ll_iterate(ll_element *head, size_t iter, int print)
{
    size_t i;
    ll_element *p;

    p = head;
    for (i = 0; i < iter; i++) {
        p = p->next;
    }
}



int
main(int argc, char const *argv[])
{
    unsigned long long n_elements = atoi(argv[2]);
    unsigned long long iterations = atoi(argv[3]);
    int shuffle = strcmp(argv[1], "shuffle") == 0;

    float working_set_size;
    char  unit;

    translate(n_elements * sizeof(ll_element), &working_set_size, &unit);
    printf("Structure Size = %lu Bytes\n",        sizeof(ll_element));
    printf("Number of Elements = %llu\n",         n_elements);
    printf("Total Working Set Size = %.1f %cB\n", working_set_size, unit);
    printf("Total Iteration = %llu\n",            iterations);
    printf("Shuffle Enabled = %s\n",              shuffle ? "Yes" : "No");

    srand(time(NULL));
    ll_element *head = ll_build(n_elements, shuffle);
    ll_iterate(head, iterations, 0);
    return 0;
}
