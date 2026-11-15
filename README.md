# Calendar Tool
A command-line calendar tool written in C, featuring Gregorian date calculations, weekday lookup, calendar printing, and date arithmetic without date/time libraries.

## Features
- Calculate the ordinal day of the year.
- Calculate the number of days between two dates.
- Determine the weekday of a give date.
- Print monthly calendars.
- Print yearly calendars with configurable column layouts.

## Requirements
- GCC, MSVC or another C compatible compiler.

## Building
Clone the repository:
```bash
git clone https://github.com/jmsmarcelo/calendar-tool-c.git
```
Compile the source files:
- GCC (Linux):
  ```bash
  gcc src/*.c -o calendar-tool
  ```
- MSVC (Windows):
  ```cmd
  cl /TC src\main.c src\calendar_cli.c src\calendar.c /Fe:calendar-tool.exe
  ```

## Usage
```txt
# Linux:
./calendar-tool <command> [arguments]

# Windows:
calendar-tool <command> [arguments]

Commands:
   days YYYY-MM-DD             Calculate the ordinal day of the year.
   days YYYY-MM-DD YYYY-MM-DD  Calculate the number of days between two dates.
   weekday YYYY-MM-DD          Get the weekday name of a given date.
   print YYYY-MM               Print a monthly calendar.
   print YYYY [columns]        Print a yearly calendar (1-6 columns (must divide 12), default: 3).
   about                       Display information about this program.
   help                        Display this help message.
```
### Examples:
```text
./calendar-tool days 2026-11-14
Output:
318th day of 2026

./calendar-tool days 2026-11-14 2026-12-25
Output:
41 days

./calendar-tool weekday 2026-11-14
Output:
saturday

./calendar-tool print 2026-11
Output:
+----------------------------+
|       november 2026        |
| su  mo  tu  we  th  fr  sa |
|  1   2   3   4   5   6   7 |
|  8   9  10  11  12  13  14 |
| 15  16  17  18  19  20  21 |
| 22  23  24  25  26  27  28 |
| 29  30                     |
+----------------------------+

./calendar-tool print 2026
Output:
+--------------------------------------------------------------------------------------+
|                                         2026                                         |
+----------------------------+----------------------------+----------------------------+
|          january           |          february          |           march            |
| su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa |
|                  1   2   3 |  1   2   3   4   5   6   7 |  1   2   3   4   5   6   7 |
|  4   5   6   7   8   9  10 |  8   9  10  11  12  13  14 |  8   9  10  11  12  13  14 |
| 11  12  13  14  15  16  17 | 15  16  17  18  19  20  21 | 15  16  17  18  19  20  21 |
| 18  19  20  21  22  23  24 | 22  23  24  25  26  27  28 | 22  23  24  25  26  27  28 |
| 25  26  27  28  29  30  31 |                            | 29  30  31                 |
+----------------------------+----------------------------+----------------------------+
|           april            |            may             |            june            |
| su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa |
|              1   2   3   4 |                      1   2 |      1   2   3   4   5   6 |
|  5   6   7   8   9  10  11 |  3   4   5   6   7   8   9 |  7   8   9  10  11  12  13 |
| 12  13  14  15  16  17  18 | 10  11  12  13  14  15  16 | 14  15  16  17  18  19  20 |
| 19  20  21  22  23  24  25 | 17  18  19  20  21  22  23 | 21  22  23  24  25  26  27 |
| 26  27  28  29  30         | 24  25  26  27  28  29  30 | 28  29  30                 |
|                            | 31                         |                            |
+----------------------------+----------------------------+----------------------------+
|            july            |           august           |         september          |
| su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa |
|              1   2   3   4 |                          1 |          1   2   3   4   5 |
|  5   6   7   8   9  10  11 |  2   3   4   5   6   7   8 |  6   7   8   9  10  11  12 |
| 12  13  14  15  16  17  18 |  9  10  11  12  13  14  15 | 13  14  15  16  17  18  19 |
| 19  20  21  22  23  24  25 | 16  17  18  19  20  21  22 | 20  21  22  23  24  25  26 |
| 26  27  28  29  30  31     | 23  24  25  26  27  28  29 | 27  28  29  30             |
|                            | 30  31                     |                            |
+----------------------------+----------------------------+----------------------------+
|          october           |          november          |          december          |
| su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa | su  mo  tu  we  th  fr  sa |
|                  1   2   3 |  1   2   3   4   5   6   7 |          1   2   3   4   5 |
|  4   5   6   7   8   9  10 |  8   9  10  11  12  13  14 |  6   7   8   9  10  11  12 |
| 11  12  13  14  15  16  17 | 15  16  17  18  19  20  21 | 13  14  15  16  17  18  19 |
| 18  19  20  21  22  23  24 | 22  23  24  25  26  27  28 | 20  21  22  23  24  25  26 |
| 25  26  27  28  29  30  31 | 29  30                     | 27  28  29  30  31         |
+----------------------------+----------------------------+----------------------------+
```

## License
This project is licensed under the MIT License. See the [LICENSE](https://github.com/jmsmarcelo/calendar-tool-c/blob/main/LICENSE) file for details.
