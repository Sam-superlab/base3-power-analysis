#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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

// Function to format time duration
void format_duration(int seconds, char *buffer)
{
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
}

// Function to estimate remaining time
void print_progress(int current, int total, time_t start_time)
{
    time_t current_time = time(NULL);
    int elapsed_seconds = (int)difftime(current_time, start_time);

    if (current == 0)
        return; // Avoid division by zero

    // Calculate estimated total time and remaining time
    double progress = (double)current / total;
    int estimated_total_seconds = (int)(elapsed_seconds / progress);
    int remaining_seconds = estimated_total_seconds - elapsed_seconds;

    char elapsed[20], remaining[20];
    format_duration(elapsed_seconds, elapsed);
    format_duration(remaining_seconds, remaining);

    // Calculate current digit ratios
    double ratio_zeros = (double)total_zeros / total_digits * 100;
    double ratio_ones = (double)total_ones / total_digits * 100;
    double ratio_twos = (double)total_twos / total_digits * 100;

    printf("\rProgress: %d/%d (%.2f%%) | Time: %s | Est. Remaining: %s",
           current, total, progress * 100, elapsed, remaining);
    printf(" | Ratios: 0s=%.2f%% 1s=%.2f%% 2s=%.2f%%",
           ratio_zeros, ratio_ones, ratio_twos);
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    int A = (argc > 1) ? atoi(argv[1]) : MAX_A;
    if (A <= 0 || A > MAX_A)
    {
        fprintf(stderr, "Invalid A value. Must be between 1 and %d\n", MAX_A);
        return 1;
    }

    // Estimate memory requirements
    size_t estimated_max_digits = calculate_digits(A);
    printf("Estimated maximum digits needed: %zu\n", estimated_max_digits);
    if (estimated_max_digits >= MAX_DIGITS)
    {
        fprintf(stderr, "Warning: Estimated digits (%zu) approaches/exceeds MAX_DIGITS (%d)\n",
                estimated_max_digits, MAX_DIGITS);
        printf("Continue? (y/n): ");
        char response;
        scanf(" %c", &response);
        if (response != 'y' && response != 'Y')
        {
            return 1;
        }
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

    time_t start_time = time(NULL);
    printf("\nStarting computation...\n");

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

        // Print progress every 1000 steps or at specific milestones
        if (a % 1000 == 0 || a == A ||
            (a < 1000 && (a % 100 == 0)) || // More frequent updates early on
            (a == 1 || a == 10 || a == 100))
        {
            print_progress(a, A, start_time);
            printf("\n");
        }

        // Prepare next power unless this is the last iteration
        if (a < A)
        {
            len = multiply_by_2_base3(digits, len);
            if (len >= MAX_DIGITS)
            {
                fprintf(stderr, "\nExceeded maximum digits at a=%d\n", a);
                fclose(csv);
                return 1;
            }
        }
    }

    time_t end_time = time(NULL);
    int total_seconds = (int)difftime(end_time, start_time);
    char duration[20];
    format_duration(total_seconds, duration);

    printf("\n\nFinal Results for A=%d:\n", A);
    printf("Total digits processed: %llu\n", total_digits);
    printf("Distribution:\n");
    printf("Zeros: %.6f%%\n", (double)total_zeros / total_digits * 100);
    printf("Ones:  %.6f%%\n", (double)total_ones / total_digits * 100);
    printf("Twos:  %.6f%%\n", (double)total_twos / total_digits * 100);
    printf("\nTotal computation time: %s\n", duration);

    fclose(csv);
    return 0;
}