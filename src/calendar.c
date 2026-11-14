#include "calendar.h"

static const int const month_sizes[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const char *const weekday_names[7] = {
    "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"
};

static int is_leap_year(int year) {
    return (
        year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)
    );
}

int days_in_month(int year, int month) {
    return month_sizes[month]
            + (month == 1 && is_leap_year(year));
}

int day_of_year(int year, int month, int day) {
    int days = day + (month > 1 && is_leap_year(year));

    for(int i = 0; i < month; i++) {
        days += month_sizes[i];
    }

    return days;
}

long days_at_date(int year, int month, int day) {
    return (
        (year - 1) * 365
        + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400)
        + day_of_year(year, month, day)
    );
}

int is_valid_date(int year, int month, int day) {
    return !(
        month < 0 || month > 11
        || year < 1
        || day < 1 || day > days_in_month(year, month)
    );
}

int weekday_of_date(int year, int month, int day) {
    return days_at_date(year, month, day) % 7;
}

const char *get_weekday(int year, int month, int day) {
    return weekday_names[weekday_of_date(year, month, day)];
}