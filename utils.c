#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define TBPS (1ull * 1000 * 1000 * 1000 * 1000)
#define GBPS (1ull * 1000 * 1000 * 1000)
#define MBPS (1ull * 1000 * 1000)
#define KBPS (1ull * 1000)
void
translate(uint64_t number, float *f_number, char *unit)
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
