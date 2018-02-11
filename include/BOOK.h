#ifndef BOOK_H
#define BOOK_H
#include "date.h"
typedef struct
{
    char author[100], publisher[100], title[100], ISBN[14];
    int cID;
    int noOfCopies,currentAvailable;
    int bookCategory;
    int needSave;
    int borrowed_count;
    date* date_of_publish;
} Book;
int find_Isbn(char*);
int isBook_Available(char*);
void deleteBook(Book*);
void load_books_from_database();
void delete_book_menu();
void insert_book_menu();
void search_book_menu();
void saveBook(Book*);
void add_new_copies_menu();
void bookTask(int);
char* get_category(int cat);

Book* books_array[1000];
Book* deleted_books_array[1000];

int total_books_count,total_books_deleted_count;

Book* createNewBook();

#endif // BOOK_H
