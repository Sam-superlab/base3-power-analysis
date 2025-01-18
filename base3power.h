#ifndef BASE3POWER_H
#define BASE3POWER_H

#include <stdint.h>

// Constants for maximum supported values
#define MAX_A 1000000
#define MAX_DIGITS 650000 // Slightly more than 0.63 * MAX_A

// Core function declarations
int multiply_by_2_base3(uint8_t *digits, int len);
void count_digits(const uint8_t *digits, int len,
                  unsigned long long *cnt0,
                  unsigned long long *cnt1,
                  unsigned long long *cnt2);

// Structure to hold digit counts
typedef struct
{
    unsigned long long zeros;
    unsigned long long ones;
    unsigned long long twos;
} DigitCounts;

#endif // BASE3POWER_H