/*
 * Backward Shift-Add for string matching with k mismatches.
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long word;
#define W (8 * (int)sizeof(word))

static unsigned char *P;
static size_t m;
static int L; /* number of bits per state */
static word Hbit, Lmask, Hmask, IV, Vmask;
static word B[256];

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

    /* Initialize constants */
    Hbit = (word)1 << (L-1);
    Lmask = Hmask = IV = 0;
    for (i = 0; i < m; i++) {
        Lmask = (Lmask << L) | 1;
        Hmask = (Hmask << L) | Hbit;
        IV = (IV << L) | (Hbit + k);
    }
    Vmask = Hmask | (Hmask - Lmask);

    /* Incidence matrix */
    for (c = 0; c < 256; c++)
        B[c] = Lmask;
    for (i = 0; i < m; i++)
        B[P[m-1 - i]] ^= (word)1 << i*L;
}

size_t exec(unsigned char *T, size_t n, size_t k)
{
    word D;
    size_t l, r, occ = 0;

    r = m-1;
    while (r < n) {
        l = r - m + 1;
        D = IV - B[T[r]];
        while (r > l && (D & Hmask)) {
            r = r - 1;
            D = (D << L) - (B[T[r]] & (D << 1));
        }
        if (D & Hmask) {
            /* match at T[l..l+m-1] */
            occ++;
        }
        r = r + m;
    }

    return occ;
}
