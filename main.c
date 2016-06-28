#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define NPAD       6
#define N_ELEMENTS 10
// #define N_ELEMENTS 1000
// #define N_ELEMENTS 1000000

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

#define TBPS (1ull * 1000 * 1000 * 1000 * 1000)
#define GBPS (1ull * 1000 * 1000 * 1000)
#define MBPS (1ull * 1000 * 1000)
#define KBPS (1ull * 1000)
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

    for (i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        ll_element t = array[j];
        array[j] = array[i];
        array[i] = t;
    }

    for (i = 0; i < n; i++) {
        ll_element *elt = &array[i];
        elt->next = &array[i+1];
    }

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
    while (head) {
        head->index = 0x234251f;
        head = head->next;
    }
}


int
main(int argc, char const *argv[])
{

    printf("Size of struct = %lu\n", sizeof(ll_element));
    ll_element *head = ll_build();
    srand(time());
    ll_shuffle(head, N_ELEMENTS);
    ll_print(head);

    unsigned long long begin = rdtsc();
    ll_process(head);
    unsigned long long end = rdtsc();
    unsigned long long cycles = end - begin;

    float working_set_size;
    char  unit;
    unit_translate(N_ELEMENTS * sizeof(ll_element), &working_set_size, &unit);
    printf("Total Working Set Size: %.1f %cB\n", working_set_size, unit);
    printf("Total Cycles Time: %llu\n",          cycles);
    printf("Cycle Per Element: %lu\n",           (long unsigned) (cycles / N_ELEMENTS));
    return 0;
}
