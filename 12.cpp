#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DIGITS 1000  

typedef struct {
    int len;            /* number of used digits */
    int d[MAX_DIGITS];  /* digits in reverse order (least significant first) */
} BigInt;

/* initialize BigInt to value v (v >= 0, small) */
void bigint_init(BigInt *a, int v) {
    for (int i = 0; i < MAX_DIGITS; ++i) a->d[i] = 0;
    a->len = 0;
    if (v == 0) { a->d[0] = 0; a->len = 1; return; }
    while (v > 0) {
        a->d[a->len++] = v % 10;
        v /= 10;
    }
}

/* multiply BigInt a by small int m (m >= 0) */
void bigint_mul_small(BigInt *a, int m) {
    if (m == 0) { a->len = 1; a->d[0] = 0; return; }
    int carry = 0;
    for (int i = 0; i < a->len; ++i) {
        long prod = (long)a->d[i] * m + carry;
        a->d[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry > 0) {
        a->d[a->len++] = carry % 10;
        carry /= 10;
    }
}

/* compute factorial n! into BigInt res (n >= 0) */
void factorial_bigint(int n, BigInt *res) {
    bigint_init(res, 1);
    for (int i = 2; i <= n; ++i) bigint_mul_small(res, i);
}

/* print BigInt in normal decimal order */
void bigint_print(const BigInt *a) {
    for (int i = a->len - 1; i >= 0; --i) putchar('0' + a->d[i]);
}

/* compute log2(n!) via summing log2(i) to get accurate exponent (double) */
double log2_factorial(int n) {
    if (n < 2) return 0.0;
    double s = 0.0;
    for (int i = 2; i <= n; ++i) s += log2((double)i);
    return s;
}

int main(void) {
    int n1 = 25; /* all permutations: 25! */
    int n2 = 24; /* effectively unique (25!/25 = 24!) */

    BigInt f25, f24;
    factorial_bigint(n1, &f25);
    factorial_bigint(n2, &f24);

    double log2_f25 = log2_factorial(n1);
    double log2_f24 = log2_factorial(n2);

    printf("25! (exact) = ");
    bigint_print(&f25);
    printf("\n");

    /* print approximate as 2^x (show x with a few decimals) */
    printf("25! ˜ 2^(%.6f)  (˜ 2^%.0f if rounded)\n\n", log2_f25, floor(log2_f25 + 0.5));

    printf("24! (exact) = ");
    bigint_print(&f24);
    printf("\n");
    printf("24! ˜ 2^(%.6f)  (˜ 2^%.0f if rounded)\n\n", log2_f24, floor(log2_f24 + 0.5));

    /* also print scientific-style approximation using pow(2, fractional) and exponent */
    double exp25 = floor(log2_f25);
    double frac25 = log2_f25 - exp25;
    double mant25 = pow(2.0, frac25);  /* mantissa between 1 and <2 */
    printf("25! ˜ %.6f × 2^(%.0f)  => about %.6fe%.0f (base-2 mantissa form)\n",
           mant25, exp25, mant25, exp25);

    double exp24 = floor(log2_f24);
    double frac24 = log2_f24 - exp24;
    double mant24 = pow(2.0, frac24);
    printf("24! ˜ %.6f × 2^(%.0f)\n", mant24, exp24);

    return 0;
}

