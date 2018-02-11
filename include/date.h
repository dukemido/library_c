#ifndef DATE_H
#define DATE_H

typedef struct
{
    int day,month,year;
} date;

date* createDate();
date* get_date_now();
date* date_from_string(char*);
char* date_to_string(date*);
void printDate(date*);
void scan_date(date*);
void copy_date(date* dest,date* src);
int validate_date(date*);
#endif // DATE_H
