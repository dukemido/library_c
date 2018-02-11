#include "date.h"
#include <time.h>

void scan_date(date* ret_date)
{
    do
    {
        printf("Enter the day:");
        scanf("%d",&ret_date->day);
        printf("Enter the month:");
        scanf("%d",&ret_date->month);
        printf("Enter the year:");
        scanf("%d",&ret_date->year);
    }
    while(!validate_date(ret_date));
}

/**
    Returns a date with the passed params added to the current date
    ya3ny bta5od el date bt3 dlw2ty w tzwd 3aleh el paramaters ely ana mdhalo feh function :D
*/
date* get_future_date_from_now(int day,int month,int year)
{
    date* gdate = get_date_now();/** current date */
    date* ptr = (date*)malloc(sizeof(date));/** date to return */

    ptr->day = gdate->day+day;
    ptr->month = gdate->month+month;
    ptr->year = gdate->year+year;

    while(ptr->day > 31)/** validation : tool ma al day > 31 htn2s 31 w tzwd 1 month*/
    {
        ptr->day-=31;
        ptr->month++;
    }
    while(ptr->month>12)/** validation : tool ma al month > 12 htn2s 12 w tzwd 1 year */
    {
        ptr->month-=12;
        ptr->year++;
    }
    return ptr;
}

date* get_date_now()
{
    time_t t = time(NULL);/** gets current date */
    struct tm tm = *localtime(&t); /** puts it in the struct tm */

    /** Allocates new date to store the current date. */
    date* ptr = (date*)malloc(sizeof(date));

    ptr->day =tm.tm_mday;/** puts the current day in ptr->date */
    ptr->month = tm.tm_mon+1;/** current month */
    ptr->year = tm.tm_year+1900; /** current year */
    return ptr;
}
/**
* return 1 if d1> d2
* return -1 if d2 > d1
* return 0 if d1 = d2
EXAMPLE : d1 = 11-1-2019
        : d2 = 19-12-2017
*/

int cmpdates(date* d1,date* d2)
{
    if(d1->year > d2->year) return 1;
    if(d1->year < d2->year) return -1;
    if(d1->year == d2->year)
    {
        if(d1->month > d2->month) return 1;
        if(d1->month < d2->month) return -1;
        if(d1->month==d2->month)
        {
            if(d1->day > d2->day) return 1;
            if(d1->day < d2->day) return -1;
            if(d1->day == d2->day) return 0;
        }
    }
}

void printDate(date* myDate)
{
    printf("\nDate year:%d - month:%d - day:%d\n",myDate->year,myDate->month,myDate->day);
}

/**
    Gets the string and splits it using the strtok method..
    Example 12-3-2010 =>day:12 month:3 year:2010
    and validates the date.. and returns current date if the date isnt valid.
 */
date* date_from_string(char* str2)
{
    date* ptr = malloc(sizeof(date));
    char *token;

    char str[strlen(str2)+1];
    strcpy(str,str2);

    token = strtok(str, "-");
    ptr->day = atoi(token);

    token = strtok(NULL, "-");
    ptr->month = atoi(token);

    token = strtok(NULL, "-");
    ptr->year = atoi(token);

    if(!validate_date(ptr))
    {
        printf("Invalid date loaded.. returned current date\n");
        return get_date_now();
    }
    return ptr;
}

int validate_date(date* d)
{
    if(d->day >= 31 || d->day <1) return 0;
    if(d->month > 12 || d->month <1) return 0;
    if(d->year >= 2200 || d->year <1700) return 0;
    return 1;
}
void copy_date(date* dest,date* src)
{
    *dest = *src;
}

/** badelo date

* byrg3ly 'day-month-year' example : 19-1-1999

*/
char* date_to_string(date* myDate)
{
    char *d2 = malloc(20);
    sprintf(d2,"%d-%d-%d",myDate->day,myDate->month,myDate->year);
    return d2;
}
