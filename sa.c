/*
 * Shift-Add for string matching with k mismatches.
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long word;
#define W (8 * sizeof(word))

static unsigned int B;
static word lim, ovmask, mask, T[256];

/* ceil(log2(x)) */
static unsigned int clog2(size_t x)
{
    unsigned int i;
    for (i = 0; ((size_t)1 << i) < x; i++);
    return i;
}

void prep(unsigned char *pat, size_t m, size_t k)
{
    size_t i;

    B = clog2(k+1) + 1;
    if (m*B > W) {
        fprintf(stderr, "need m*B=%ld > %d=W bits\n", (long)(m*B), (int)W);
        exit(EXIT_FAILURE);
    }

    lim = (word)k << (m-1)*B;
    ovmask = 0;
    for (i = 1; i <= m; i++)
        ovmask = (ovmask << B) | ((word)1 << (B-1));
    lim += (word)1 << (m-1)*B;
    for (i=0; i < 256; i++) T[i] = ovmask >> (B-1);
    for (i = 1; *pat != '\0'; i <<= B) {
        T[*pat] &= ~i;
        pat++;
    }

    mask = (m*B == W) ? ~(word)0 : (word)(i-1);
}

size_t exec(unsigned char *text, size_t n, size_t k)
{
    word state, overflow;
    size_t i, occ = 0; (void)k;

    state = mask & ~ovmask;
    overflow = ovmask;

    occ = 0;
    for (i = 0; i < n; i++) {
        state = ((state << B) + T[text[i]]) & mask;
        overflow = ((overflow << B) | (state & ovmask)) & mask;
        state &= ~ovmask;
        if ((state + overflow) < lim) {
            /* match at T[i-m+1..i] */
            occ++;
        }
    }

    return occ;
}
