#include <stdio.h>
#include <string.h>
#include "calendar_cli.h"
#include "calendar.h"

#define CMD_TABLE_COUNT (sizeof(cmd_table) / sizeof(cmd_table[0]))

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
    return 0;
}

static int cmd_help(int argc, char *argv[]) {
    return 0;
}

static const cmd_entry_t cmd_table[] = {
    {"days", cmd_days},
    {"weekday", cmd_weekday},
    {"print", cmd_print},
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