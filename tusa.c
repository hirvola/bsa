/*
 * Tuned Shift-Add for string matching with k mismatches.
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long word;
#define W (8 * (int)sizeof(word))

static unsigned char *P;
static size_t m;
static int L; /* number of bits per state */
static word mask, IV, B[256];

/* ceil(log2(x)) */
int clog2(size_t x)
{
    int i;
    for (i = 0; ((size_t)1 << i) < x; i++);
    return i;
}

void prep(unsigned char *P_, size_t m_, size_t k)
{
    int c;
    size_t i;
    P = P_; m = m_;

    L = clog2(k+1) + 1;
    if (m*L > W) {
        fprintf(stderr, "need m*L=%lu > %d=W bits\n", (unsigned long)(m*L), W);
        exit(42);
    }

    /* Incidence matrix */
    mask = 0;
    for (i = 0; i < m; i++)
        mask = (mask << L) | 1;
    for (c = 0; c < 256; c++)
        B[c] = mask;
    for (i = 0; i < m; i++)
        B[P[i]] ^= (word)1 << i*L;

    /* Constants */
    mask = (word)1 << (m*L-1);
    IV = ((word)1 << (L-1)) - (k+1);
}

size_t exec(unsigned char *T, size_t n, size_t k)
{
    word D;
    size_t i, occ = 0;

    D = ~(word)0;
    for (i = 0; i < n; i++) {
        D = ((D << L) | IV) + (B[T[i]] & ~(D << 1));
        if (!(D & mask)) {
            /* match at T[i-m+1..i] */
            occ++;
        }
    }

    return occ;
}
