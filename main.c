#include <stdio.h>
#include <stdlib.h>


#define NPAD          6
#define ELEMENT_COUNT 1000000

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

ll_element *
ll_build(void)
{
    int i;
    ll_element *head, *curr, *prev;

    head = curr = prev = NULL;

    for (i = 0; i < ELEMENT_COUNT; i++) {
        curr = malloc(sizeof(ll_element));
        if (!curr) { return NULL; }

        curr->index = i;
        if (i == 0) { head = prev = curr; } else {
            prev->next = curr;
            prev = curr;
        }
    }

    return head;
}

void
ll_print(ll_element *head)
{
    while (head) {
        printf("Index: %lu\n", head->index);
        head = head->next;
    }
}

void ll_process(ll_element *head)
{
    while (head) {
        head->index = 0x234251f;
        head = head->next;
    }
}


int
main(int argc, char const *argv[])
{

    ll_element *head = ll_build();
    // ll_print(head);
    // printf("Size of struct = %lu\n", sizeof(ll_element));

    unsigned long long begin = rdtsc();
    ll_process(head);
    unsigned long long end = rdtsc();
    unsigned long long cycles = end - begin;

    printf("Total Cycles Time: %llu\n", cycles);
    printf("Cycle Per Element: %lu\n", (long unsigned) (cycles / ELEMENT_COUNT));
    return 0;
}
