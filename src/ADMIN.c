#include "ADMIN.h"
#include "BOOK.h"
#include "BORROW.h"
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void administrativeTasks(int subTask)
{
    switch(subTask)
    {
    case 1:
    {
        admin_overdue_books();
        break;
    }
    case 2:
    {
        admin_most_popular();
        break;
    }
    default:
    {
        printf("Unknown administrative subtask %d\n",subTask);
        break;
    }
    }
}
void admin_most_popular()
{
    int i,j;
    for(i=0; i<total_books_count; i++)
        books_array[i]->borrowed_count = 0;

    /**
        Calculates the borrow count for each book.
        Loops over all borrows and adds 1 borrow to the book.
    */
    for(i = 0; i<total_borrows_count; i++)/** Loops over all books */
        for(j=0; j<total_books_count; j++)/** Finds the borrowed book in the books array */
        {
            if(!strcmp(borrows_array[i]->ISBN,books_array[j]->ISBN))
            {
                books_array[j]->borrowed_count++;
                break;
            }
        }

    /** Sorted array */
    Book* sorted_array[total_books_count];

    for(i=0; i<total_books_count; i++)
    {
        sorted_array[i] = malloc(sizeof(Book));
        *sorted_array[i] = *books_array[i];
    }

    Book* temp = malloc(sizeof(Book));
    for(i=0; i<total_books_count; i++)
    {
        /*
         * Place the currently selected element array[i]
         * to its correct place.
         */
        for(j=i+1; j<total_books_count; j++)
        {
            /*
             * Swap if currently selected array element
             * is not at its correct position.
             */
            if(sorted_array[i]->borrowed_count < sorted_array[j]->borrowed_count)
            {
                *temp = *sorted_array[i];
                *sorted_array[i] = *sorted_array[j];
                *sorted_array[j]= *temp;
            }
        }
    }
    system("@cls");
    printf("Most Popular books\n--------------\n");
    for(i = 0; i<MIN(total_books_count,5); i++)
        printf("Book : %s - Borrows Count: %d\n",sorted_array[i]->title,sorted_array[i]->borrowed_count);
    printf("\n--------------\n");

}
void admin_overdue_books()
{
    printf("\n------------\n");
    int i,c=0;
    for(i = 0; i<total_borrows_count; i++)
    {
        if(cmpdates(borrows_array[i]->return_date,get_date_now()) == -1
                && borrows_array[i]->still_borrowed)
        {
            print_late(borrows_array[i]);
            c++;
        }
    }
    printf("Results found : %d\n------------\n",c);
}
/**

*/
void print_late(Borrowing *pBorrow)
{
    printf("Member Id:%d didn`t return the book %s - due return date %s\n",
           pBorrow->CARDID,
           pBorrow->ISBN,
           date_to_string(pBorrow->return_date));
}
