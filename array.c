#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARRAY_COLUMN_SIZE 5000
#define ARRAY_ROW_SIZE    5000

int
main(int argc, char const *argv[])
{

    uint32_t i, j;
    uint8_t  **array;

    array = calloc(ARRAY_ROW_SIZE, sizeof(uint8_t *));
    for (i = 0; i < ARRAY_ROW_SIZE; i++) {
        array[i] = calloc(ARRAY_COLUMN_SIZE, sizeof(uint8_t *));
    }

    for (i = 0; i < ARRAY_ROW_SIZE; i++) {
        for (j = 0; j < ARRAY_COLUMN_SIZE; j++) {
            array[i][j] = 'F';
        }
    }

    return 0;
}
