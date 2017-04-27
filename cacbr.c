/*
 * Approximate CBNDM for circular string matching with k mismatches.
 * Modified version with reversed bit-vectors and rotation direction.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long word;
#define W (8 * (int)sizeof(word))

/* Rotate bit vector 'v' of width 'w' to the left by 'c' bits */
#define ROL(v, c, w) (((v) << (c)) | ((v) >> ((w)-(c))))

static unsigned char *P;
static size_t m;
static word B[256];

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
    P = P_; m = m_;

    if (m > W) {
        fprintf(stderr, "need m=%lu > %d=W bits\n", (unsigned long)m, W);
        exit(42);
    }

    /* Incidence matrix */
    memset(B, 0, sizeof(B));
    for (i = 0; i < m; i++)
        B[P[i]] |= (word)1 << (m-i);
}

size_t exec(unsigned char *T, size_t n, size_t k)
{
    word D;
    size_t i, j, e, occ = 0;

    i = 0; D = 0;
    while (i <= n-m) {
        e = j = 0;
        while (e <= k && j < m) {
            e++; j = j + 1;
            D = B[T[i+m-j]];
            while (D && j < m) {
                j = j + 1;
                D = ROL(D, 1, m) & B[T[i+m-j]];
            }
        }
        if ((j == m && D) || e <= k) {
            if (verify(&T[i], k)) {
                /* match at T[i..i+m-1] */
                occ++;
            }
        }
        i = i + (m-j+1);
    }

    return occ;
}