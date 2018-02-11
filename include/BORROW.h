#ifndef BORROW_H
#define BORROW_H
#include "date.h"

#define TRUE 1
#define FALSE 0

typedef struct
{
    char ISBN[14];
    int CARDID;
    date* date_issued;
    date* return_date;
    date* actual_return;
    int needSave;
    int still_borrowed;// Is the book returned or no..
    int borrow_UID;
} Borrowing;
void borrowTask(int);
void print_borrow(Borrowing*);
void returnBook();
void borrowBook();
void save_borrows();
void returnBook();

Borrowing* borrows_array[1000];

int total_borrows_count;

void load_borrows_from_database();
Borrowing* createNewBorrow();
#endif // BORROW_H
