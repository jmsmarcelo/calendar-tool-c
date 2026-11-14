#ifndef CALENDAR_H
#define CALENDAR_H

int days_in_month(int year, int month);
int is_valid_date(int year, int month, int day);
int day_of_year(int year, int month, int day);
long days_at_date(int year, int month, int day);

#endif