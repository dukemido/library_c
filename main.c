#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "BOOK.h"
#include "MEMBERS.h"
#include "date.h"
#include "ADMIN.h"
// We're using this library as JSON PARSER https://github.com/DaveGamble/cJSON


void print_menu();
void executeTask(int);
int filesExists(const char*);

/**
* Checks if the file exists by trying to open it.
* returns 1 if the file exists.
* returns 0 if the file doesn`t exist.
*/
int fileExists(const char* filename)
{
    int exists = 0;
    FILE *file;
    file = fopen(filename, "r");
    if(file != 0) /** Found the file*/
    {
        exists = 1;
        fclose(file);
    }
    return exists;
}

int main()
{
    load_books_from_database();/** gets the files from folder database//books (.json) files */
    load_members_from_database();/** gets the files from folder database//members (.json) files */
    load_borrows_from_database();/** gets the files from folder database//borrows (.json) files */

    admin_most_popular();/** print most popular books */
    while(1)/** 1 so the program keeps running in an infinite loop */
    {
        print_menu();/** prints the main menu */
    }
    return 0;
}

/**
* This method prints the tasks numbers and waits for the task number
* Then executes it from executeTask.
*/
void print_menu()
{
    printf("Menu :\n1.Book Management.\n2.Member Management.\n3.Borrow Management.\n4.Administrative actions.\n5.Save changes.\n6.Exit\n-----\n");
    printf("Please choose what you want to do : ");
    int task;
    scanf("%d",&task);
    system("@cls");
    executeTask(task);
}

/** This method executes the task and takes the param int which is the task id.
*/

void executeTask(int task)
{

    switch(task)
    {
    case 1:/** Books Management*/
    {
        int subTask;
        printf("1.Insert a book\n2.Search.\n3.Add new copies.\n4.Delete.\n5.Print all books\nPress any other number to be back\nWhat do you want to :");
        scanf("%d",&subTask);
        if(subTask >=1 && subTask <= 5)
            bookTask(subTask);
        else
            system("@cls");
        break;
    }
    case 2:/** Members Management*/
    {
        int subTask;
        printf("1.Register a member\n2.Delete a member.\n3.Print all members.\nPress any other number to be back\nWhat do you want to :");
        scanf("%d",&subTask);
        if(subTask >=1 && subTask <= 3)
            memberTask(subTask);
        else
            system("@cls");
        break;
    }

    case 3:/** Borrow Management*/
    {
        int subTask;
        printf("1.Borrowing book.\n2.Returning book.\n3.Print all borrows.\nPress any other number to be back\nWhat do you want to do:");
        scanf("%d",&subTask);
        if(subTask >= 1 && subTask<= 3)
            borrowTask(subTask);
        else
            system("@cls");
        break;
    }

    case 4:/** Admin roles.*/
    {
        int subTask;
        printf("1.Overdue books.\n2.Most popular books.\nPress any other number to be back\nWhat do you want to :");
        scanf("%d",&subTask);
        if(subTask >= 1 && subTask<= 2)
            administrativeTasks(subTask);
        else
            system("@cls");
        break;
    }
    case 5:/** Save */
    {
        save_delete_books();
        save_delete_members();
        save_borrows();
        break;
    }
    case 6:
        exit(0);
        break;
    default:
        printf("Unknown task id : %d\n",task);
        break;
    }
}
