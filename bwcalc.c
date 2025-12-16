/*
 * bwcalc.c
 * Bandwidth Calculator
 * Author: Bryan Candiliere
 * Date: October 20, 2025
 * Description: CLI calculator to determine download times and bandwidth requirements.
 * License: MIT (see LICENSE file)
 */

#include "bwcalc.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (check_arguments(argc, argv) != 0)
    {
        return 1;
    }

    if (STRCASECMP(argv[1], "about") == 0)
    {
        about_application();
        return 0;
    }

    /* Values for formatted ouput */
    long long raw_days, raw_hours, raw_minutes;
    double raw_seconds;
    double raw_size, raw_speed;
    char raw_size_unit[SIZE_STR];
    char raw_speed_unit[SPEED_STR];

    /* Prompt user for file size and convert to bits */
    double total_file_size_bits = get_file_size_bits(&raw_size, raw_size_unit);

    if (STRCASECMP(argv[1], "time") == 0)
    {
        /* Prompt user for connection speed and convert to bps */
        double connection_speed_bps = get_connection_speed_bps(&raw_speed, raw_speed_unit);
        /* Calculate total time in seconds */
        double total_time_seconds = calculate_time_seconds(total_file_size_bits, connection_speed_bps);
        /* Format/Output the result */
        format_time_output(raw_size, raw_size_unit, raw_speed, raw_speed_unit, total_time_seconds);
    }
    else if (STRCASECMP(argv[1], "speed") == 0)
    {
        /* Prompt user for time values and convert to seconds */
        double total_time_seconds = get_time_seconds(&raw_days, &raw_hours, &raw_minutes, &raw_seconds);
        /* Check for error code reurned from get_time_seconds() */
        if (total_time_seconds < 0.0)
        {
            return 1;
        }
        /* Calculate total bandwidth in bps */
        double total_bandwidth_bps = calculate_speed_bps(total_file_size_bits, total_time_seconds);
        /* Format/Output the result */
        format_speed_output(raw_size, raw_size_unit, raw_days, raw_hours, raw_minutes, raw_seconds,
                            total_bandwidth_bps);
    }

    return 0;
}

/* Check for correct command-line arguments */
int check_arguments(int count, char *value[])
{
    if (count == 1)
    {
        printf("Bandwidth Calculator usage:\n");
        printf("Calculate Time:  ./bwcalc time\n");
        printf("Calculate Speed: ./bwcalc speed\n");
        printf("Show Info:       ./bwcalc about\n");
        return 1;
    }
    if (count != 2)
    {
        printf("Error: Incorrect number of arguments.\n\n");
        printf("Bandwidth Calculator usage:\n");
        printf("Calculate Time:  ./bwcalc time\n");
        printf("Calculate Speed: ./bwcalc speed\n");
        printf("Show Info:       ./bwcalc about\n");
        return 1;
    }
    if (STRCASECMP(value[1], "time") != 0 && STRCASECMP(value[1], "speed") != 0 &&
        STRCASECMP(value[1], "about") != 0)
    {
        printf("Error: Invalid argument '%s'.\n\n", value[1]);
        printf("Bandwidth Calculator usage:\n");
        printf("Calculate Time:  ./bwcalc time\n");
        printf("Calculate Speed: ./bwcalc speed\n");
        printf("Show Info:       ./bwcalc about\n");
        return 1;
    }
    if (STRCASECMP(value[1], "time") == 0 || STRCASECMP(value[1], "speed") == 0)
    {
        printf("Calculator mode: %s\n\n", value[1]);
    }

    return 0;
}

/* Prompt user for file size and convert to bits for calculation (both modes) */
double get_file_size_bits(double *size_value, char *size_unit_str)
{
    char unit;
    double size;

    printf("========================\n");
    printf("File Size Units\n");
    printf("========================\n");

    printf("B)  Bytes\n");
    printf("K)  Kibibytes\n");
    printf("M)  Mebibytes\n");
    printf("G)  Gibibytes\n");
    printf("T)  Tebibytes\n");
    do
    {
        printf("\nChoose a file size unit: ");
        scanf(" %c", &unit);
        unit = toupper(unit);
    }
    while (unit != 'B' && unit != 'K' && unit != 'M' && unit != 'G' && unit != 'T');

    do
    {
        printf("\nEnter a file size: ");
        if (scanf("%lf", &size) != 1 || size < 0.0)
        {
            while (getchar() != '\n')
                ;       /* clear invalid input */
            size = -1;  /* reset size to stay in loop */
        }
    }
    while (size <= 0.0);

    printf("\n");

    /*
     * Store raw user input values back to main() via pointers,
     * then perform the final conversion on the local 'size' variable.
     */
    switch (unit)
    {
        case 'B':
            *size_value = size;
            strcpy(size_unit_str, "B");
            size *= (BYTE_FACTOR * BITS_PER_BYTE);
            break;
        case 'K':
            *size_value = size;
            strcpy(size_unit_str, "KiB");
            size *= (KIBIBYTE_FACTOR * BITS_PER_BYTE);
            break;
        case 'M':
            *size_value = size;
            strcpy(size_unit_str, "MiB");
            size *= (MEBIBYTE_FACTOR * BITS_PER_BYTE);
            break;
        case 'G':
            *size_value = size;
            strcpy(size_unit_str, "GiB");
            size *= (GIBIBYTE_FACTOR * BITS_PER_BYTE);
            break;
        case 'T':
            *size_value = size;
            strcpy(size_unit_str, "TiB");
            size *= (TEBIBYTE_FACTOR * BITS_PER_BYTE);
            break;
        default:
            printf("Error: Invalid unit '%c'.\n", unit);
            exit(1);
    }

    /* Return value of size in bits */
    return size;
}

/* Promp user for connection speed and convert to bps for calculation (time mode) */
double get_connection_speed_bps(double *speed_value, char *speed_unit_str)
{
    char unit;
    double speed;

    printf("========================\n");
    printf("Connection Speed Units\n");
    printf("========================\n");

    printf("B)  bps\n");
    printf("K)  Kbps\n");
    printf("M)  Mbps\n");
    printf("G)  Gbps\n");
    printf("T)  Tbps\n");
    do
    {
        printf("\nChoose a connection speed unit: ");
        scanf(" %c", &unit);
        unit = toupper(unit);
    }
    while (unit != 'B' && unit != 'K' && unit != 'M' && unit != 'G' && unit != 'T');

    do
    {
        printf("\nEnter a connection speed: ");
        if (scanf("%lf", &speed) != 1 || speed < 0.0)
        {
            while (getchar() != '\n')
                ;        /* clear invalid input */
            speed = -1;  /* reset speed to stay in loop */
        }
    }
    while (speed <= 0.0);

    printf("\n");

    /*
     * Store raw user input values back to main() via pointers,
     * then perform the final conversion on the local 'speed' variable.
     */
    switch (unit)
    {
        case 'B':
            *speed_value = speed;
            strcpy(speed_unit_str, "bps");
            speed *= BPS_FACTOR;
            break;
        case 'K':
            *speed_value = speed;
            strcpy(speed_unit_str, "Kbps");
            speed *= KBPS_FACTOR;
            break;
        case 'M':
            *speed_value = speed;
            strcpy(speed_unit_str, "Mbps");
            speed *= MBPS_FACTOR;
            break;
        case 'G':
            *speed_value = speed;
            strcpy(speed_unit_str, "Gbps");
            speed *= GBPS_FACTOR;
            break;
        case 'T':
            *speed_value = speed;
            strcpy(speed_unit_str, "Tbps");
            speed *= TBPS_FACTOR;
            break;
        default:
            printf("Error: Invalid unit '%c'.\n", unit);
            exit(1);
    }

    /* Return value of speed in bps */
    return speed;
}

/* Calculate total time in seconds (time mode) */
double calculate_time_seconds(double bits, double bps)
{
    double total_time_seconds;
    total_time_seconds = bits / bps;
    return total_time_seconds;
}

/* Format and output time result (time mode) */
void format_time_output(double size_value, char *size_unit_str, double speed_value,
                        char *speed_unit_str, double total_time_seconds)
{
    /* Round total seconds to the nearest whole second and convert to long long */
    long long total_time_seconds_rounded = (long long) round(total_time_seconds);

    /* Get days from total seconds then get ramaining seconds to check for hours */
    long long days = total_time_seconds_rounded / SECONDS_PER_DAY;
    long long remaining_seconds = total_time_seconds_rounded % SECONDS_PER_DAY;

    /* Get hours from remaining seconds then get remaining seconds to check for minutes */
    long long hours = remaining_seconds / SECONDS_PER_HOUR;
    remaining_seconds = remaining_seconds % SECONDS_PER_HOUR;

    /* Get minutes from remaining seconds then get remaining seconds to check for seconds */
    long long minutes = remaining_seconds / SECONDS_PER_MINUTE;
    remaining_seconds = remaining_seconds % SECONDS_PER_MINUTE;

    /* Get fractional seconds */
    double fractional_seconds = total_time_seconds - (double) total_time_seconds_rounded;

    /* Combined seconds and fractional seconds for final output */
    double final_seconds = (double) remaining_seconds + fractional_seconds;

    printf("-------------------------\n");

    printf("A file transfer of %.2f %s at %.2f %s will take:\n\n", size_value, size_unit_str,
           speed_value, speed_unit_str);

    /* Print days if greater than 0 */
    if (days > 0)
    {
        printf("%lld day%s, ", days, (days == 1 ? "" : "s"));
    }
    /* Print hours if greater than 0 or always if days were printed */
    if (hours > 0 || days > 0)
    {
        printf("%lld hour%s, ", hours, (hours == 1 ? "" : "s"));
    }
    /* Print minutes if greater than 0 or always if days/hours were printed */
    if (minutes > 0 || hours > 0 || days > 0)
    {
        printf("%lld minute%s, ", minutes, (minutes == 1 ? "" : "s"));
    }
    /* Always print seconds */
    printf("%.2f second%s\n", final_seconds, (final_seconds == 1.0 ? "" : "s"));
}

/* Prompt user for time values and convert to seconds for calculation (speed mode) */
double get_time_seconds(long long *days, long long *hours, long long *minutes, double *seconds)
{
    /* Initialize variables to fail loop condition */
    long long days_input = -1;
    long long hours_input = -1;
    long long minutes_input = -1;
    double seconds_input = -1.0;

    printf("========================\n");
    printf("Time Input\n");
    printf("========================\n");

    do
    {
        int invalid = 0;
        printf("Enter days (e.g., 1, or 0 if none): ");
        if (scanf("%lld", &days_input) != 1 || days_input < 0)
        {
            invalid = 1;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            /* reject all non-whitespace characters after valid input */
            if (!isspace((unsigned char)c))
            {
                invalid = 1;
            }
        }
        if (invalid)
        {
            days_input = -1;
            printf("Invalid input.\n");
        }
    }
    while (days_input < 0);

    do
    {
        int invalid = 0;
        printf("Enter hours (e.g., 1, or 0 if none): ");
        if (scanf("%lld", &hours_input) != 1 || hours_input < 0)
        {
            invalid = 1;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            if (!isspace((unsigned char)c))
            {
                invalid = 1;
            }
        }
        if (invalid)
        {
            hours_input = -1;
            printf("Invalid input.\n");
        }
    }
    while (hours_input < 0);

    do
    {
        int invalid = 0;
        printf("Enter minutes (e.g., 1, or 0 if none): ");
        if (scanf("%lld", &minutes_input) != 1 || minutes_input < 0)
        {
            invalid = 1;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            if (!isspace((unsigned char)c))
            {
                invalid = 1;
            }
        }
        if (invalid)
        {
            minutes_input = -1;
            printf("Invalid input.\n");
        }
    }
    while (minutes_input < 0);

    do
    {
        printf("Enter seconds (e.g., 1.5, or 0 if none): ");
        if (scanf("%lf", &seconds_input) != 1 || seconds_input < 0.0)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            seconds_input = -1.0;
        }
    }
    while (seconds_input < 0.0);

    printf("\n");

    /* Store raw user input values back to main() via pointers */
    *days = days_input;
    *hours = hours_input;
    *minutes = minutes_input;
    *seconds = seconds_input;

    /* Calculate total time in seconds */
    double total_time_seconds = (double) days_input * SECONDS_PER_DAY +
                                (double) hours_input * SECONDS_PER_HOUR +
                                (double) minutes_input * SECONDS_PER_MINUTE + seconds_input;

    /* Check for potential division by zero error if all user entered values are zero */
    if (total_time_seconds <= 0.0)
    {
        printf("-------------------------\n");
        printf("Error: Total transfer time must be greater than zero.\n");
        return -1.0;  /* Return an error value to check for in main() */
    }

    /* Return total time in seconds */
    return total_time_seconds;
}

/* Calculate total bandwidth in bps (speed mode) */
double calculate_speed_bps(double bits, double seconds)
{
    double total_speed_bps;
    total_speed_bps = bits / seconds;
    return total_speed_bps;
}

/* Format and output speed result (speed mode) */
void format_speed_output(double size_value, char *size_unit_str, long long days, long long hours,
                         long long minutes, double seconds, double total_bandwidth_bps)
{
    printf("-------------------------\n");

    printf("To transfer a file of %.2f %s in ", size_value, size_unit_str);
    /* Print days if greater than 0 */
    if (days > 0)
    {
        printf("%lld day%s, ", days, (days == 1 ? "" : "s"));
    }
    /* Print hours if greater than 0 or always if days were printed */
    if (hours > 0 || days > 0)
    {
        printf("%lld hour%s, ", hours, (hours == 1 ? "" : "s"));
    }
    /* Print minutes if greater than 0 or always if days/hours were printed */
    if (minutes > 0 || hours > 0 || days > 0)
    {
        printf("%lld minute%s, ", minutes, (minutes == 1 ? "" : "s"));
    }
    /* Always print seconds */
    printf("%.2f second%s, a bandwidth of:\n\n", seconds, (seconds == 1.0 ? "" : "s"));

    /* Format output based on bandwidth value */
    if (total_bandwidth_bps < KBPS_FACTOR)
    {
        printf("%.2f bps is required\n", total_bandwidth_bps);
        return;
    }
    if (total_bandwidth_bps < MBPS_FACTOR)
    {
        printf("%.2f Kbps is required\n", total_bandwidth_bps / KBPS_FACTOR);
        return;
    }
    if (total_bandwidth_bps < GBPS_FACTOR)
    {
        printf("%.2f Mbps is required\n", total_bandwidth_bps / MBPS_FACTOR);
        return;
    }
    if (total_bandwidth_bps < TBPS_FACTOR)
    {
        printf("%.2f Gbps is required\n", total_bandwidth_bps / GBPS_FACTOR);
        return;
    }
    printf("%.2f Tbps is required\n", total_bandwidth_bps / TBPS_FACTOR);
}

void about_application()
{
    printf("%s\n", PROG);
    printf("Version: %s\n", VERSION);
    printf("Author:  %s\n", AUTHOR);
}
