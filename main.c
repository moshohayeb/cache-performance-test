#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>





#define TBPS (1ull * 1000 * 1000 * 1000 * 1000)
#define GBPS (1ull * 1000 * 1000 * 1000)
#define MBPS (1ull * 1000 * 1000)
#define KBPS (1ull * 1000)

#define NPAD       1
#define N_ELEMENTS 1000
#define N_ITER     100000

typedef struct _elt {
    struct _elt *next;
    long int    index;
    long int    pad[NPAD];
} ll_element;

static __inline__ unsigned long long
rdtsc(void)
{
    unsigned long long int x;

    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

void
unit_translate(uint64_t number, float *f_number, char *unit)
{
    if (number > TBPS) {
        *f_number = number * 1.0 / TBPS;
        *unit = 'T';
    } else if (number > GBPS) {
        *f_number = number * 1.0 / GBPS;
        *unit = 'G';
    } else if (number > MBPS) {
        *f_number = number * 1.0 / MBPS;
        *unit = 'M';
    } else if (number > KBPS) {
        *f_number = number * 1.0 / KBPS;
        *unit = 'K';
    } else {
        *f_number = number * 1.0;
        *unit = ' ';
    }
}


void
ll_shuffle(ll_element *array, size_t n)
{
    size_t i;

    // shuffle the array
    for (i = 0; i < n; i++) {
        int idx1 = rand() % n;
        int idx2 = rand() % n;

        ll_element t = array[idx1];
        array[idx1] = array[idx2];
        array[idx2] = t;
    }

    // reconnect the dots
    for (i = 0; i < n - 1; i++) {
        array[i].next = &array[i + 1];
    }

    array[n - 1].next = NULL;
}

ll_element *
ll_build(void)
{
    int i;
    ll_element *p, *prev, *curr;

    p = prev = calloc(N_ELEMENTS, sizeof(ll_element));
    if (!p) { return NULL; }

    for (i = 0; i < N_ELEMENTS; i++) {
        curr = (ll_element *) (p + i);
        curr->index = i;
        if (i != 0) {
            prev->next = curr;
            prev = curr;
        }
    }

    return p;
}

void
ll_print(ll_element *head)
{
    while (head) {
        printf("Index: %lu\n", head->index);
        head = head->next;
    }
}

void
ll_process(ll_element *head)
{
    int i;

    for (i = 0; i < N_ITER; i++) {
        ll_element *h = head;

        while (h) {
            h->index = 0x234251f;
            h = h->next;
        }
    }
}


int
main(int argc, char const *argv[])
{
    srand(time(NULL));

    printf("Size of struct = %lu\n", sizeof(ll_element));
    ll_element *head = ll_build();
    ll_shuffle(head, N_ELEMENTS);
    // ll_print(head);

    printf("Starting...\n");
    unsigned long long begin = rdtsc();
    ll_process(head);
    unsigned long long end = rdtsc();
    unsigned long long cycles = end - begin;
    printf("Finished...\n");

    float working_set_size;
    char  unit;
    unit_translate(N_ELEMENTS * sizeof(ll_element), &working_set_size, &unit);
    printf("Total Working Set Size: %.1f %cB\n", working_set_size, unit);
    printf("Total Cycles Time: %llu\n",          cycles);
    printf("Cycle Per Element: %lu\n",           (long unsigned) (cycles / N_ELEMENTS / N_ITER));
    return 0;
}
