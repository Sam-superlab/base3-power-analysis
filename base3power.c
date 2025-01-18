#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "base3power.h"

// Add these global variables to track the sums
unsigned long long total_digits = 0;
unsigned long long total_zeros = 0;
unsigned long long total_ones = 0;
unsigned long long total_twos = 0;

// Function to calculate total digits for 2^a in base 3
int calculate_digits(int a)
{
    return 1 + floor(a * log(2) / log(3));
}

int main(int argc, char *argv[])
{
    int A = (argc > 1) ? atoi(argv[1]) : MAX_A;
    if (A <= 0 || A > MAX_A)
    {
        fprintf(stderr, "Invalid A value. Must be between 1 and %d\n", MAX_A);
        return 1;
    }

    FILE *csv = fopen("frequency_data.csv", "w");
    if (!csv)
    {
        perror("Failed to open CSV file");
        return 1;
    }
    fprintf(csv, "power,zeros,ones,twos,zeros_percent,ones_percent,twos_percent\n");

    static uint8_t digits[MAX_DIGITS];
    int len = 1;
    digits[0] = 2; // 2^1 in base 3

    for (int a = 1; a <= A; a++)
    {
        unsigned long long cnt0, cnt1, cnt2;
        count_digits(digits, len, &cnt0, &cnt1, &cnt2);

        // Update running totals
        total_zeros += cnt0;
        total_ones += cnt1;
        total_twos += cnt2;
        total_digits += len;

        // Calculate ratios
        double ratio_zeros = (double)total_zeros / total_digits;
        double ratio_ones = (double)total_ones / total_digits;
        double ratio_twos = (double)total_twos / total_digits;

        // Write to CSV
        fprintf(csv, "%d,%llu,%llu,%llu,%.6f,%.6f,%.6f\n",
                a, cnt0, cnt1, cnt2,
                ratio_zeros * 100,
                ratio_ones * 100,
                ratio_twos * 100);

        if (a % 1000 == 0)
        {
            printf("Processed power 2^%d\n", a);
            printf("Current ratios: 0s=%.6f%% 1s=%.6f%% 2s=%.6f%%\n",
                   ratio_zeros * 100,
                   ratio_ones * 100,
                   ratio_twos * 100);
        }

        // Prepare next power unless this is the last iteration
        if (a < A)
        {
            len = multiply_by_2_base3(digits, len);
            if (len >= MAX_DIGITS)
            {
                fprintf(stderr, "Exceeded maximum digits at a=%d\n", a);
                fclose(csv);
                return 1;
            }
        }
    }

    printf("\nFinal Results for A=%d:\n", A);
    printf("Total digits processed: %llu\n", total_digits);
    printf("Distribution:\n");
    printf("Zeros: %.6f%%\n", (double)total_zeros / total_digits * 100);
    printf("Ones:  %.6f%%\n", (double)total_ones / total_digits * 100);
    printf("Twos:  %.6f%%\n", (double)total_twos / total_digits * 100);

    fclose(csv);
    return 0;
}