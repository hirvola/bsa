/*
 * A naive algorithm for circular string matching with k mismatches.
 */

#include <stddef.h>

static unsigned char *P;
static size_t m;

void prep(unsigned char *P_, size_t m_, size_t k)
{
    P = P_; m = m_; (void)k;
}

size_t exec(unsigned char *T, size_t n, size_t k)
{
    size_t i, j, r, neq, occ = 0;
    for (i = 0; i <= n-m; i++) {
        for (r = 0; r < m; r++) {
            neq = 0;
            for (j = 0; j < m; j++) {
                if (P[(r+j) % m] != T[i+j] && ++neq > k)
                    break;
            }
            if (j == m) {
                occ++;
                break;
            }
        }
    }
    return occ;
}
