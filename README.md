# Bandwidth Calculator
### A command-line calculator written in C that can calculate file transfer times and bandwidth requirements.

# 📜 Description

This calculator utility is written in C for use in the command-line on any operating system.

There are three arguments that can be provided when launching the application:
```
./bwcalc time
./bwcalc speed
./bwcalc about
```
## ⏳ Time
#### This command starts the application in time calculation mode, determining how long a file transfer will take given a specific network speed.
The user is prompted for the following information in sequence:

- File Size Unit Selection: The user chooses a unit (e.g., B, KiB, MiB, GiB, TiB).
- File Size Input: The user enters the numerical value for the file size.
- Connection Speed Unit Selection: The user chooses a unit (e.g., bps, Kbps, Mbps, Gbps, Tbps).
- Connection Speed Input: The user enters the numerical value for the connection speed.

Internally, the application converts the file size to bits and the speed to bits per second (bps) before calculating the total transfer time. The final result is converted to a human-readable format (Days, Hours, Minutes, Seconds) and reported to the user.

Additionally, pointers are used to pass the user provided values from the `get_file_size_bits` function back to variables in `main` so they can be reused to format the output in yet another function, `format_time_output`.

## 🚀 Speed
#### This command starts the application in speed calculation mode, determining what bandwidth is required to transfer a file in a specific amount of time.
The user is prompted for the following information in sequence:

- File Size Unit Selection: The user chooses a unit (e.g., B, KiB, MiB, GiB, TiB).
- File Size Input: The user enters the numerical value for the file size.
- Time Input (Days, Hours, Minutes, Seconds): The user is prompted to provide a specific duration for the file transfer.

Internally, the application converts the file size to bits and the total duration to seconds before calculating the required bandwidth. The bandwidth result is then converted and displayed using the appropriate speed units (bps, Kbps, etc.)

Additionally, in this mode as well, pointers are used to pass the user provided values from the `get_time_seconds` function back to variables in `main` so they too can be reused to format the output in another function called `format_speed_output`.

## 🔎 About
This mode displays information about the application including version number and author's name (me).

## 💾 Files
This project consists of the following files:
```
bwcalc.c
bwcalc.h
Makefile
README.md
```
- **bwcalc.c**
  - This file contains the the main implementation for this project. It includes the main function of the application, and calls to all helper and calculation functions. It also includes necessary library headers and references `bwcalc.h`.

- **bwcalc.h**
  - This header file declares all the function prototypes, constants, macros, and type definitions used by `bwcalc.c`.

- **Makefile**
  - This file contains instructions used by the `make` utility to automate the compilation process. This can also be done manually without a makefile. I used `GCC` compiler while developing this application. The command needed to compile the application manually is: `cc bwcalc.c -o bwcalc -lm`. In this case the `-lm` flag is needed to link the `math.h` library.
 
- **README.md**
  - This documentation file! 📖

## ❗ Additional Information
#### File sizes are measured using <ins>Binary Units</ins> (IEC Standard) (KiB, MiB, GiB, TiB) rather than <ins>Decimal Units</ins> (KB, MB, GB, TB).

This is to ensure precision in file sizes. Binary units directly reflect the powers of two used by computer memory and storage (2<sup>10</sup>, 2<sup>20</sup>, 2<sup>30</sup>, etc.) and to avoid confusion caused by the approximation used in Decimal units (10<sup>3</sup>, 10<sup>6</sup>, 10<sup>9</sup>, etc.).

#### So, to wrap things up, units in this application are defined as follows:

| File Size Units (Binary / Base-2) | Value (Bytes) |
| :------- | :------- |
| Byte (base unit) | 1.0 |
| Kibibyte (KiB) | 1,024.0 |
| Mebibyte (MiB) | 1,048,576.0 |
| Gibibyte (GiB) | 1,073,741,824.0 |
| Tebibyte (TiB) | 1,099,511,627,776.0 |

| Bandwidth Unit (Decimal / Base-10) | Value (bits per second) |
| :------- | :------- |
| bps (base unit) | 1.0 |
| Kbps | 1,000.0 |
| Mbps | 1,000,000.0 |
| Gbps | 1,000,000,000.0 |
| Tbps | 1,000,000,000,000.0 |

> [!TIP]
> [This is an excellent read](https://www.corsair.com/us/en/explorer/diy-builder/storage/mebibyte-vs-megabyte-whats-the-difference/) on the subject by **CORSAIR** memory.

> [!NOTE]
> - **Cross-Platform Portability**: A conditional compilation block uses preprocessor directives to define a single macro (STRCASECMP) for cross-platform, case-insensitive string comparison. This allows compatibility with both Microsoft (MSVC) and POSIX (GCC/Clang) compilers.
