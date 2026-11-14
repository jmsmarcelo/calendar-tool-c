#include <stdio.h>
#include <string.h>
#include "calendar_cli.h"
#include "calendar.h"

#define CMD_TABLE_COUNT (sizeof(cmd_table) / sizeof(cmd_table[0]))

// Number of columns for yearly calendar (must divide 12)
#define PRINT_MAX_COLUMNS 6
#define PRINT_DEFAULT_COLUMNS 3

typedef int (*cmd_f)(int argc, char *argv[]);

typedef struct cmd_entry_type {
    const char *name;
    cmd_f func;
} cmd_entry_t;

typedef struct date_type {
    int year;
    int month;
    int day;
} date_t;

typedef struct date_field_layout_type {
    int offset;
    int width;
    int separator_offset;
} date_field_layout_t;

static int parse_ndigits(const char *str, size_t len) {
    int value = 0;
    for(size_t i = 0; i < len; i++) {
        unsigned char c = str[i] - '0';
        if(c > 9) return -1;
        value *= 10;
        value += c;
    }
    return value;
}

static int parse_digits(const char *str, int max) {
    int value = 0;
    while(*str) {
        unsigned char c = *str - '0';
        if(c > 9) return -1;
        value *= 10;
        value += c;
        if(value > max) return -1;
        str++;
    }
    return value;
}

static int date_parse(date_t *date, const char *str) {
    if(date == NULL || str == NULL) return 0;

    int parts_count = 0;
    int date_parts[3] = {0};

    // Mapping for extracting fields
    static const date_field_layout_t field_layout[3] = {
        {0, 4, 4},  // year
        {5, 2, 7},  // month
        {8, 2, 10}  // day
    };
    for(int i = 0; i < 3; i++) {
        date_parts[i] = parse_ndigits(str + field_layout[i].offset, field_layout[i].width);
        if(date_parts[i] < 0) return 0;
        parts_count++;

        switch(str[field_layout[i].separator_offset]) {
            case '\0': break;
            case '-': continue;
            default: return 0;
        }
        break;
    }

    date->year = 0;
    date->month = 0;
    date->day = 0;

    switch(parts_count) {
        case 3:
            if(str[10] != '\0') return 0;
            date->day = date_parts[2];
        case 2:
            date->month = date_parts[1];
        case 1:
            date->month--;  // set to zero-based
            date->year = date_parts[0];
            return parts_count;
        case 0:
        default:
            return 0;
    }
}

static const char *get_ordinal(int n) {
    switch((n % 100) - 11) {
        case 0:
        case 1:
        case 2:
            return "th";
        default:
            switch((n % 10) - 1) {
                case 0: return "st";
                case 1: return "nd";
                case 2: return "rd";
                default: return "th";
            }
    }
}

static void print_repeated(int count, const char *str, const char *prefix, const char *suffix) {
    printf("%s", prefix);
    while(count-- > 0) printf("%s", str);
    printf("%s", suffix);
}

static int print_month(const date_t *date) {
    int first_weekday = weekday_of_date(date->year, date->month, 1);
    int days_month = days_in_month(date->year, date->month);
    char title[32];
    int title_len = 28;
    title_len -= snprintf(title, sizeof(title), "%s %d", get_month(date->month), date->year);
    int padding = title_len / 2;

    print_repeated(28, "-", "+", "+\n");
    printf("|%*s%s%*s|\n", padding, "", title, title_len - padding, "");
    printf("| su  mo  tu  we  th  fr  sa |\n");

    int day = 1;
    while(day <= days_month) {
        putchar('|');
        for(int j = 0; j < 7; j++) {
            printf(
                " %2.d ", ((day > 1 || j >= first_weekday) && day <= days_month) ? day++ : 0
            );
        }
        printf("|\n");
    }
    print_repeated(28, "-", "+", "+\n");
    return 0;
}

static int print_year(int year, int columns) {
    int first_weekday[PRINT_MAX_COLUMNS];
    int month_lengths[PRINT_MAX_COLUMNS];
    int day[PRINT_MAX_COLUMNS];
    int month = 0;
    int base_weekday = weekday_of_date(year, 0, 1);

    char title[32];
    int title_len = 29 * columns - 1;

    print_repeated(title_len, "-", "+", "+\n|");

    title_len -= snprintf(title, sizeof(title), "%d", year);
    int padding = title_len / 2;

    printf("%*s%s%*s|\n", padding, "", title, title_len - padding, "");
    print_repeated(columns, "----------------------------+", "+", "\n");

    int rows = 12 / columns;
    for(int i = 0; i < rows; i++) {
        putchar('|');
        for(int j = 0; j < columns; j++) {
            title_len = 28;
            title_len -= snprintf(title, sizeof(title), "%s", get_month(month + j));
            padding = title_len / 2;
            printf("%*s%s%*s|", padding, "", title, title_len - padding, "");

            day[j] = 1;
            first_weekday[j] = base_weekday;
            month_lengths[j] = days_in_month(year, month + j);
            base_weekday = (base_weekday + month_lengths[j]) % 7;
        }
        print_repeated(columns, " su  mo  tu  we  th  fr  sa |", "\n|", "\n");

        int stop = 0;
        int finished[PRINT_MAX_COLUMNS] = {0};
        while(stop < columns) {
            putchar('|');
            for(int j = 0; j < columns; j++) {
                for(int k = 0; k < 7; k++) {
                    printf(
                        " %2.d ",
                        ((day[j] > 1 || k >= first_weekday[j]) && day[j] <= month_lengths[j]) ? day[j]++ : 0
                    );
                }
                putchar('|');

                if(!finished[j] && day[j] > month_lengths[j]) {
                    finished[j] = 1;
                    stop++;
                }
            }
            putchar('\n');
        }
        print_repeated(columns, "----------------------------+", "+", "\n");

        month += columns;
    }
    return 0;
}

static int cmd_days(int argc, char *argv[]) {
    switch(argc - 3) {
        case 0:
            date_t date;
            if(date_parse(&date, argv[2]) != 3
                    || !is_valid_date(date.year, date.month, date.day)) {
                printf("Invalid or incomplete date, please, enter '%s help' for more information.\n", argv[0]);
                return -1;
            }
            int day = day_of_year(date.year, date.month, date.day);
            printf("%d%s day of %d\n", day, get_ordinal(day), date.year);
            return 0;
        case 1:
            date_t dates[2];
            if(date_parse(&dates[0], argv[2]) != 3
                    || date_parse(&dates[1], argv[3]) != 3
                    || !is_valid_date(dates[0].year, dates[0].month, dates[0].day)
                    || !is_valid_date(dates[1].year, dates[1].month, dates[1].day)) {
                printf("Invalid or imcomplete date, please, enter '%s help' for more information.\n", argv[0]);
                return -1;
            }
            long days = days_at_date(dates[1].year, dates[1].month, dates[1].day)
                        - days_at_date(dates[0].year, dates[0].month, dates[0].day);
            printf("%ld day%s\n", days, (days == 1 || days == -1) ? "" : "s");
            return 0;
        default:
            printf("Invalid number of arguments, please, enter '%s help' for more information.\n", argv[0]);
            return -1;
    }
}

static int cmd_weekday(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Invalid number of arguments, please, enter '%s help' for more information.\n", argv[0]);
        return -1;
    }
    date_t date;
    if(date_parse(&date, argv[2]) != 3
            || !is_valid_date(date.year, date.month, date.day)) {
        printf("Invalid or incomplete date, please, enter '%s help' for more information.\n", argv[0]);
        return -1;
    }
    printf("%s\n", get_weekday(date.year, date.month, date.day));
    return 0;
}

static int cmd_print(int argc, char *argv[]) {
    int columns = 0;
    date_t date;
    switch(argc - 3) {
        case 1:
            columns = parse_digits(argv[3], PRINT_MAX_COLUMNS);
            if(columns < 1 || 12 % columns != 0) {
                printf("Invalid number of columns, please, enter '%s help' for more information.\n", argv[0]);
                return -1;
            }
        case 0:
            switch(date_parse(&date, argv[2])) {
                case 2:
                    if(!is_valid_date(date.year, date.month, 1)) {
                        printf("Invalid date, please, enter '%s help' for more information.\n", argv[0]);
                        return -1;
                    }
                    if(columns > 0) {
                        printf("It is not need number of columns, please, enter '%s help' for more information.\n", argv[0]);
                        return -1;
                    }
                    return print_month(&date);
                case 1:
                    if(date.year > 0) {
                        return print_year(date.year, columns > 0 ? columns : PRINT_DEFAULT_COLUMNS);
                    }
                case 0:
                default:
                    printf("Invalid date, please, enter '%s help' for more information.\n", argv[0]);
                    return -1;
            }
        default:
            printf("Invalid number of arguments, please, enter '%s help' for more information.\n", argv[0]);
            return -1;
    }
}

static int cmd_about(int argc, char *argv[]) {
    if(argc > 2) {
        printf("It is not need arguments, please, enter '%s help' for more information.\n", argv[0]);
        return -1;
    }

    printf(
        "+-----------------------------------------------+\n"
        "|                 Calendar Tool                 |\n"
        "| A command-line calendar utility written in C. |\n"
        "|                                               |\n"
        "|                  Source code                  |\n"
        "| https://github.com/jmsmarcelo/calendar-tool-c |\n"
        "+-----------------------------------------------+\n"
    );
    return 0;
}

static int cmd_help(int argc, char *argv[]) {
    if(argc > 2) {
        printf("It is not need arguments, please, enter '%s help' for more information.\n", argv[0]);
        return -1;
    }
    printf(
        "Usage: %s <command> [arguments]\n"
        "\n"
        "Commands:\n"
        "   days YYYY-MM-DD             Calculate the ordinal day of the year.\n"
        "   days YYYY-MM-DD YYYY-MM-DD  Calculate the number of days between two dates.\n"
        "   weekday YYYY-MM-DD          Get the weekday name of a given date.\n"
        "   print YYYY-MM               Print a monthly calendar.\n"
        "   print YYYY [columns]        Print a yearly calendar (1-%d columns (must divide 12), default: %d).\n"
        "   about                       Display information about this program.\n"
        "   help                        Display this help message.\n"
        "\n"
        "Examples:\n"
        "   %s days 2026-11-14\n"
        "   %s days 2026-11-14 2026-12-25\n"
        "   %s weekday 2026-11-14\n"
        "   %s print 2026-11\n"
        "   %s print 2026\n"
        "   %s print 2026 6\n",
        argv[0],
        PRINT_MAX_COLUMNS,
        PRINT_DEFAULT_COLUMNS,
        argv[0],
        argv[0],
        argv[0],
        argv[0],
        argv[0],
        argv[0]
    );
    return 0;
}

static const cmd_entry_t cmd_table[] = {
    {"days", cmd_days},
    {"weekday", cmd_weekday},
    {"print", cmd_print},
    {"about", cmd_about},
    {"help", cmd_help}
};

static int exec_cmd(int argc, char *argv[]) {
    for(size_t i = 0; i < CMD_TABLE_COUNT; i++) {
        if(strcmp(argv[1], cmd_table[i].name) == 0)
            return cmd_table[i].func(argc, argv);
    }
    printf("Invalid command, please, enter '%s help' for more information.\n", argv[0]);
    return -1;
}

int calendar_cli(int argc, char *argv[]) {
    if(argc > 1)
        return exec_cmd(argc, argv);

    printf("Please, enter a valid command or '%s help' for more information.\n", argv[0]);
    return -1;
}