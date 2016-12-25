#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 2000

struct foo {
    int a;
    long fill[7];
    int b;
};

void
multiply_normal(double mul1[N][N], double mul2[N][N])
{
    uint32_t i, j, k;
    double   res[N][N];


    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            res[i][j] = 0;
            for (k = 0; k < N; k++) {
                res[i][j] += mul1[i][k] * mul2[k][j];
            }
        }
    }
}

void
multiply_transposed(double mul1[N][N], double mul2[N][N])
{
    uint32_t i, j, k;
    double   tmp[N][N];
    double   res[N][N];

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            tmp[i][j] = mul2[j][i];
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            res[i][j] = 0;
            for (k = 0; k < N; k++) {
                res[i][j] = mul1[i][k] * tmp[j][k];
            }
        }
    }
}

void
init_array(double mul[N][N])
{
    uint32_t i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            mul[i][j] = i * j;
        }
    }
}


int
main(int argc, char const *argv[])
{
    double mul1[N][N];
    double mul2[N][N];

    struct foo f;

    f.a = 3;
    f.b = 34;
    printf("size of foo = %d\n", sizeof(f));
    init_array(mul1);
    init_array(mul2);

    if (argc > 1) {
        multiply_transposed(mul1, mul2);
    } else {
        multiply_normal(mul1, mul2);
    }

    return 0;
}
