#include <stdio.h>
#include <stdlib.h>
#include "base3power.h"

// Multiply digits[] (base-3) by 2 in place
int multiply_by_2_base3(uint8_t *digits, int len)
{
    int carry = 0;
    for (int i = 0; i < len; i++)
    {
        int product = digits[i] * 2 + carry;
        digits[i] = product % 3; // new digit in base 3
        carry = product / 3;
    }
    // If there's a leftover carry, append a new digit
    if (carry > 0)
    {
        digits[len] = carry; // carry is always 1 or 2 for base 3
        len++;
    }
    return len;
}

// Count how many 0s, 1s, and 2s are in digits[]
void count_digits(const uint8_t *digits, int len,
                  unsigned long long *cnt0,
                  unsigned long long *cnt1,
                  unsigned long long *cnt2)
{
    unsigned long long z = 0, o = 0, t = 0;
    for (int i = 0; i < len; i++)
    {
        if (digits[i] == 0)
            z++;
        else if (digits[i] == 1)
            o++;
        else
            t++;
    }
    *cnt0 = z;
    *cnt1 = o;
    *cnt2 = t;
}

int main(int argc, char *argv[])
{
    // Parse input for exponent limit A
    int A = (argc > 1) ? atoi(argv[1]) : MAX_A;
    if (A <= 0 || A > MAX_A)
    {
        fprintf(stderr, "Error: Exponent must be between 1 and %d\n", MAX_A);
        return 1;
    }

    // Initialize digit array
    static uint8_t digits[MAX_DIGITS];
    int len = 1;
    digits[0] = 2; // 2^1 in base 3

    // Open CSV file for frequency data
    FILE *csv = fopen("frequency_data.csv", "w");
    if (!csv)
    {
        fprintf(stderr, "Error: Could not create frequency_data.csv\n");
        return 1;
    }

    // Write CSV header
    fprintf(csv, "power,zeros,ones,twos,total_digits,zeros_percent,ones_percent,twos_percent\n");

    // Global accumulators for final summary
    DigitCounts total = {0, 0, 0};

    // Process each power and write to CSV
    for (int a = 1; a <= A; a++)
    {
        // Count digits for current power
        unsigned long long z, o, t;
        count_digits(digits, len, &z, &o, &t);

        // Update totals
        total.zeros += z;
        total.ones += o;
        total.twos += t;

        // Calculate percentages for current power
        unsigned long long current_total = z + o + t;
        double z_percent = 100.0 * z / current_total;
        double o_percent = 100.0 * o / current_total;
        double t_percent = 100.0 * t / current_total;

        // Write data to CSV
        fprintf(csv, "%d,%llu,%llu,%llu,%llu,%.6f,%.6f,%.6f\n",
                a, z, o, t, current_total, z_percent, o_percent, t_percent);

        // Print progress every 1000 steps
        if (a % 1000 == 0)
        {
            printf("Progress: Computed 2^%d (using %d digits)\n", a, len);
        }

        // Multiply by 2 for next iteration (except for last iteration)
        if (a < A)
        {
            len = multiply_by_2_base3(digits, len);
            if (len >= MAX_DIGITS)
            {
                fprintf(stderr, "Error: Exceeded maximum digits at a=%d\n", a);
                fclose(csv);
                return 1;
            }
        }
    }

    fclose(csv);

    // Print final summary
    unsigned long long total_digits = total.zeros + total.ones + total.twos;
    printf("\nFinal Results for powers 2^1 through 2^%d in base 3:\n", A);
    printf("Total digits processed: %llu\n", total_digits);
    printf("Zeros: %llu (%.6f%%)\n", total.zeros,
           100.0 * total.zeros / total_digits);
    printf("Ones:  %llu (%.6f%%)\n", total.ones,
           100.0 * total.ones / total_digits);
    printf("Twos:  %llu (%.6f%%)\n", total.twos,
           100.0 * total.twos / total_digits);
    printf("\nFrequency data has been written to frequency_data.csv\n");

    return 0;
}