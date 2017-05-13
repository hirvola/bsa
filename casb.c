/*
 * Approximate SBNDM for circular string matching with k mismatches.
 * Detects circular matches by allowing an extra error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long word;
#define W (8 * sizeof(word))

static unsigned char *P;
static size_t m;

static word E, B[256];

/* Verify a circular occurrence allowing k mismatches */
static int verify(unsigned char *S, size_t k)
{
    size_t neq, i, j;
    for (i = 0; i < m; i++) {
        neq = 0;
        for (j = 0; j < m; j++) {
            if (S[j] != P[(i+j) % m] && ++neq > k)
                break;
        }
        if (j == m) return 1;
    }
    return 0;
}

void prep(unsigned char *P_, size_t m_, size_t k)
{
    size_t i;
    P = P_; m = m_; (void)k;

    if (m+1 > W) {
        fprintf(stderr, "need m+1=%ld > %d=W bits\n", (long)m+1, (int)W);
        exit(EXIT_FAILURE);
    }

    /* Initialization vector */
    E = ~(word)0 >> (W-m-1);

    /* Incidence matrix */
    memset(B, 0, sizeof(B));
    for (i = 0; i < m; i++)
        B[P[i]] |= (word)1 << i;
}

size_t exec(unsigned char *T, size_t n, size_t k)
{
    word D;
    size_t i, j, e, occ = 0;

    i = 0;
    while (i <= n-m) {
        D = E;
        e = j = 0;
        while (e <= k+1 && j < m) {
            j = j + 1;
            D = (D >> 1) & B[T[i+m-j]];
            if (D == 0) {
                e = e + 1;
                D = E;
            }
        }
        if (j == m && e <= k+1) {
            if (verify(&T[i], k)) {
                /* match at T[i..i+m-1] */
                occ++;
            }
        }
        i = i + (m-j+1);
    }

    return occ;
}
