/*
 * bwcalc.h
 * Bandwidth Calculator header file
 * Author: Bryan Candiliere
 * Date: October 20, 2025
 * License: MIT (see LICENSE file)
 */

#ifndef BWCALC_H
#define BWCALC_H

/* Application information */
#define PROG     "Bandwidth Calculator (bwcalc)"
#define AUTHOR   "Bryan C."
#define VERSION  "1.0"

/* Bits */
#define BITS_PER_BYTE  8.0

/* File size unit factors (Binary/Base-2) */
#define BYTE_FACTOR      1.0  /* Base unit */
#define KIBIBYTE_FACTOR  1024.0
#define MEBIBYTE_FACTOR  1048576.0
#define GIBIBYTE_FACTOR  1073741824.0
#define TEBIBYTE_FACTOR  1099511627776.0

/* Connection speed unit factors (Decimal/Base-10) */
#define BPS_FACTOR   1.0  /* Base unit */
#define KBPS_FACTOR  1000.0
#define MBPS_FACTOR  1000000.0
#define GBPS_FACTOR  1000000000.0
#define TBPS_FACTOR  1000000000000.0

/* Time unit factors (seconds) */
#define SECONDS_PER_MINUTE  60
#define SECONDS_PER_HOUR    3600
#define SECONDS_PER_DAY     86400

/* Output unit arrays */
#define SIZE_STR   4
#define SPEED_STR  5

/* Cross-platform string comparison */
#ifdef _MSC_VER  /* Microsoft Visual C compiler */
#define STRCASECMP _stricmp
#else  /* POSIX-compatible compilers */
#include <strings.h>
#define STRCASECMP strcasecmp
#endif

/* Function prototypes */
int check_arguments(int count, char *value[]);
void about_application();

double get_file_size_bits(double *size_value, char *size_unit_str);
double get_connection_speed_bps(double *speed_value, char *speed_unit_str);
double calculate_time_seconds(double bits, double bps);
void format_time_output(double size_value, char *size_unit_str, double speed_value,
                        char *speed_unit_str, double total_time_seconds);

double get_time_seconds(long long *days, long long *hours, long long *minutes, double *seconds);
double calculate_speed_bps(double bits, double seconds);
void format_speed_output(double size_value, char *size_unit_str, long long days, long long hours,
                         long long minutes, double seconds, double total_bandwidth_bps);

#endif  /* BWCALC_H */
