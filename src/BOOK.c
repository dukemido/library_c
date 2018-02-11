#include "BOOK.h"
#include <stdio.h>
#include <string.h>
#include "../cJSON.h"
#include <dirent.h>
#include "BORROW.h"
#include <ctype.h>

/** Gets the string
*   Copies the string to another string
*   Lowers are the characters using tolower() function
*   Returns the string lowercase
*/
char* to_lower(char* str)
{
    int n = strlen(str);/** Gets the length of the main string (str)*/
    char *s = malloc(n+1);/** Allocates memory for the new string which i'll copy to*/
    int i;
    strcpy(s,str);/** copies the string str to s */
    for(i=0; i<n; i++)/** lowers all the characters in the string s*/
        s[i]= tolower(s[i]);
    return s;
}
/**
* deletes the current book ely wa2f 3aleh
*/
void deleteBook(Book* myBook)
{
    char path[100];
    sprintf(path,"database\\books\\%s.json",myBook->ISBN);

    if(!remove(path))/** deletes the file and returns 0 if the file is deleted*/
        printf("- Deleted book -->%s\n",myBook->title);
}
/**
    delete book menu
    asks for the user to enter the book isbn
    gets the book position
    checks if it exists
    puts the book in the deleted book array
    removes the book from the main books array
*/
void delete_book_menu()
{
    printf("Enter your book ISBN:");
    char ISBN[100];
    fflush(stdin);
    gets(ISBN);/** deleted book ISBN */

    int i,bookpos = -1;
    for(i = 0; i<total_books_count; i++)/** gets the position of the book returns -1 if the book isnt found*/
        if(!strcmp(books_array[i]->ISBN,ISBN))
        {
            bookpos = i;
            break;
        }

    if(bookpos == -1)/** book not found because bookpos is still -1*/
    {
        printf("No such ISBN for a book.\n");
        return;
    }

    if(books_array[bookpos]->currentAvailable != books_array[bookpos]->noOfCopies)
    {
        printf("There are %d borrowed copies of this book and can`t delete until these copies are back.\n"
               , books_array[bookpos]->noOfCopies - books_array[bookpos]->currentAvailable);
        return;
    }
    fflush(stdin);
    printf("Are you sure you want to delete the book \"%s\" (y/n)?\n",books_array[i]->title);
    char in;
    scanf("%c",&in);
    switch(in)
    {
    case 'y':
    case 'Y':
        break;
    default:
        return;
    }


    deleted_books_array[total_books_deleted_count++] = books_array[bookpos];/** Adds the deleted book to the deletedBOOKS Array*/
    books_array[bookpos] = books_array[--total_books_count];/** Puts the last book to the place of the current book*/
    printf("Deleted the book successfully..\n");
}

/**
* Saves all books which needSave = 1
* Deletes all books in the deletedBooks array
 */
void save_delete_books()
{
    printf("\n-------------\n");
    int i;
    int saved = 0,deleted = 0;
    for(i=0; i<total_books_count; i++)
        if(books_array[i]->needSave)
        {
            saveBook(books_array[i]);/** saves a single book if needSave = 1*/
            saved++;/** increments no of saved books by one*/
        }
    printf("Saved %d books done..\n-------------\n",saved);

    for(i=0; i<total_books_deleted_count; i++)
    {
        deleted++;
        deleteBook(deleted_books_array[i]);/**deletes the book */
    }
    total_books_deleted_count = 0;/** resets deleted book counter to zero*/
    printf("Delete %d books done..\n-------------\n",deleted);
}

/**
    Searches for the book no of available copies by ISBN
    Then returns the book position if the count of available books > 0
    If the count of available books = 0 then it returns -1
*/

int isBook_Available(char* isbn)
{
    int i,book_pos = -1;
    for(i=0; i<total_books_count; i++)/** loops over all books */
        if(!strcmp(books_array[i]->ISBN,isbn))
        {
            printf("Copies : %d\n",books_array[i]->currentAvailable);
            if(books_array[i]->currentAvailable >0)/** if the no of available copies > 0*/
                book_pos = i;/** returns the book position */
            return book_pos;
        }
    return book_pos;
}
/** Executes the task based on the user input
* 1- Book managment
* 2- Search
* 3- Add new copies
* 4- Delete
* 5- Search */
void bookTask(int type)
{
    switch(type)
    {
    case 1:// Insert a new book..
    {
        insert_book_menu();
        break;
    }
    case 2:// search for a book
    {
        search_book_menu();
        break;
    }
    case 3:
    {
        add_new_copies_menu();
        break;
    }
    case 4:// delete books
    {
        delete_book_menu();
        break;
    }
    case 5:
    {
        int i;
        for(i=0; i<total_books_count; i++)
            printBook(books_array[i]);
        break;
    }
    }
}
/**
    Increases no of copies and no of available copies
    Searches by the ISBN
*/
void add_new_copies_menu()
{
    char ISBN[14];
    printf("Enter the ISBN:");
    fflush(stdin);
    gets(ISBN);/** Gets the ISBN */

    int i;
    for(i = 0; i<total_books_count; i++)
        if(!strcmp(ISBN,books_array[i]->ISBN))/** Searches for the book by the ISBN*/
        {
            printf("Current no. of copies for the book \"%s\" is : %d\n",books_array[i]->title,books_array[i]->noOfCopies);
            printf("Enter the new no. of copies added to the library (0-1000):");
            int new_copies;
            scanf("%d",&new_copies);
            if(new_copies < 0 || new_copies > 1000)
            {
                printf("Invalid no of copies.\n");
                return;
            }
            books_array[i]->noOfCopies += new_copies;/** increases no of copies and no of available copies*/
            books_array[i]->currentAvailable+= new_copies;

            books_array[i]->needSave = TRUE;/** because no of copies increased and needs to be updated in the file */
            printf("Updated the book..\n");
            break;
        }
}
/**
    Searches by category or isbn or title
    Lets the user enter the first char of the search by
    Then asks you to enter the search keyword in-case of title or ISBN or the category number
*/
void search_book_menu()
{
    system("@cls");

    char c;
    printf("Search by ISBN(i), Title(t), Category(c), Author(a)\nEnter the first character:");
    fflush(stdin);
    scanf("%c",&c);/**gets the first character bt3 el 7aga aly anta htsearch beha */


    switch(c)
    {
    case 'c':
    case 'C':
    {
        int cat;
        fflush(stdin);
        printf("Categories 1.Programming - 2.Novel - 3.Others\n");
        printf("Enter the book Category:");
        scanf("%d",&cat);

        int i,found = 0;
        printf("Books of category : %s\n",get_category(cat));/** prints the current book category*/

        for(i = 0; i< total_books_count; i++)/** loops over all books*/
            if(books_array[i]->bookCategory  == cat)/** compares the category*/
            {
                printBook(books_array[i]);/** prints the book */
                found++;
            }
        printf("Total search results : %d\n-------------\n",found);
        break;
    }
    case 't':
    case 'T':
    {
        fflush(stdin);
        printf("Enter the book title or a part of it:");
        char search_keyword[100];
        gets(search_keyword);/** scans the part or the title */

        int i,found = 0;
        for(i = 0; i< total_books_count; i++)/** loops over all books */
        {
            if(strstr(to_lower(books_array[i]->title),to_lower(search_keyword)))/** compares the book name and the part entered all lowercase  */
            {
                printBook(books_array[i]);
                found++;
            }
        }
        printf("Total search results : %d\n-------------\n",found);
        break;
    }

    case 'a':
    case 'A':
    {
        char author[50];
        fflush(stdin);
        printf("Enter the Author Name:");
        gets(author);

        int i,found = 0;
        for(i = 0; i< total_books_count; i++)
            if(!strcmp(tolower(books_array[i]->author),tolower(author)))
            {
                printBook(books_array[i]);
                found++;
            }
        printf("Total search results : %d\n",found);
        break;
        break;
    }

    case 'i':
    case 'I':
    {
        char ISBN[14];
        fflush(stdin);
        printf("Enter the book`s ISBN:");
        gets(ISBN);

        int i,found = 0;
        for(i = 0; i< total_books_count; i++)
            if(!strcmp(books_array[i]->ISBN,ISBN))
            {
                printBook(books_array[i]);
                found++;
            }
        printf("Total search results : %d\n",found);
        break;
    }
    default:
        printf("Invalid search character..\n");
    }


}
/**
    Allocates the space for the book in memory.
    Accepts the input from the user and validates it.
    Puts the book in the array
*/
void insert_book_menu()
{
    system("@cls");
    Book* newBook = (Book*)malloc(sizeof(Book));/** allocates space for the book in the memory */
    fflush(stdin);/** To prevent the gets from being skipped*/

    /** Gets the information from he user..*/
    do
    {
        printf("Author:");
        gets(newBook->author);
    }
    while(strlen(newBook->author) == 0);/** Validates that the author isn`t empty.*/

    do
    {
        printf("Publisher:");
        gets(newBook->publisher);
    }
    while(strlen(newBook->publisher) == 0);/** Validates that the author isn`t empty.*/

    do
    {
        printf("Title:");
        gets(newBook->title);
    }
    while(strlen(newBook->title) == 0);/** Validates that the author isn`t empty.*/

    int trials = 0;
    do
    {
        if(trials > 0)
            printf("This ISBN already exists\n");
        do
        {
            printf("ISBN (10 or 13) :");
            gets(newBook->ISBN);
        }
        while(strlen(newBook->ISBN) != 13 && strlen(newBook->ISBN) != 10);
        trials++;
    }
    while(ISBN_USED(newBook->ISBN));

    do
    {
        printf("NoOfCopies (1 to 10000):");
        scanf("%d",&newBook->noOfCopies);
    }
    while(newBook->noOfCopies < 1 && newBook->noOfCopies > 10000);/** validates no of copies between 1 an 10 k*/
    newBook->currentAvailable = newBook->noOfCopies;/** sets the current available copies to no of copies*/

    do
    {
        printf("Categories 1.Programming - 2.Novel - 3.Others\n");
        printf("Category:");
        scanf("%d",&newBook->bookCategory);/** gets the category*/
    }
    while(newBook->bookCategory < 1 || newBook->bookCategory > 3);/** validates book category between 1 to 3 */

    printf("Enter the date of publish:\n");
    newBook->date_of_publish = get_date_now();
    scan_date(newBook->date_of_publish);

    printBook(newBook);
    newBook->needSave = TRUE;/** because new book needs to be saved because it doesnt exist as a json file*/
    int pos = total_books_count++;
    books_array[pos] = newBook;/** puts it in the array */
}
int ISBN_USED(char* ISBN)
{
    int i;
    for(i=0; i<total_books_count; i++)
        if(!strcmp(ISBN,books_array[i]->ISBN))
            return 1;
    return 0;
}
/**
    Saves a single book into the json file
    Called from the loop in the save_delete_books
*/
void saveBook(Book* newBook)
{
    cJSON *root;
    root = cJSON_CreateObject();/** creates the json object ely hy3mlo save */
    cJSON_AddItemToObject(root, "Title", cJSON_CreateString(newBook->title));
    cJSON_AddItemToObject(root, "ISBN", cJSON_CreateString(newBook->ISBN));
    cJSON_AddItemToObject(root, "Author", cJSON_CreateString(newBook->author));
    cJSON_AddItemToObject(root, "Publisher", cJSON_CreateString(newBook->publisher));
    cJSON_AddItemToObject(root,"NoOfCopies",cJSON_CreateNumber(newBook->noOfCopies));
    cJSON_AddItemToObject(root,"CurrentAvailable",cJSON_CreateNumber(newBook->currentAvailable));
    cJSON_AddItemToObject(root,"Category",cJSON_CreateNumber(newBook->bookCategory));
    cJSON_AddItemToObject(root,"DateOfPublish",cJSON_CreateString(date_to_string(newBook->date_of_publish)));
    /**l7d hna al json file et3ml create f root*/

    FILE* fptr;

    char fileName[30];
    sprintf(fileName,"database\\books\\%s.json",newBook->ISBN);

    char *buffer = cJSON_Print(root);/** converts the root json file to string to be saved fel file 3la tool*/
    fptr = fopen(fileName,"w");
    fprintf(fptr,buffer);
    fclose(fptr);

    newBook->needSave = FALSE;/** sets to zero because the file is saved and there are no new updates*/
    printf("- %s saved.\n",newBook->title);
}
/**
    Prints all the book info to the screen
*/
void printBook(Book* myBook)
{
    printf("\n---------------\n");
    printf("Author: %s \nPublisher: %s \nTitle: %s \nISBN: %s \nNo Of Copies: %d \nCurrent Available: %d\n",
           myBook-> author,
           myBook-> publisher,
           myBook-> title,
           myBook-> ISBN,
           myBook -> noOfCopies,
           myBook->currentAvailable);

    printf("Category : %s\nDate of publish:%s\n----------------\n",
           get_category(myBook->bookCategory),
           date_to_string(myBook->date_of_publish));
}

/**
    Returns the book category by category number
*/
char* get_category(int cat)
{
    char* myStr = (char*)malloc(20);
    switch(cat)
    {
    case 1:
        strcpy(myStr,"Programming");
        break;
    case 2:
        strcpy(myStr,"Novel");
        break;
    case 3:
        strcpy(myStr,"Others");
        break;
    }
    return myStr;
}

/** allocates space for new book in memory and puts it in the array */
Book* createNewBook(int pos)
{
    Book* ptr = (Book*)malloc(sizeof(Book));
    books_array[pos] = ptr;
    return ptr;
}

/**
* loops over all files in the directory database\\books
*/
void load_books_from_database()
{
    total_books_count  = total_books_deleted_count= 0;/** sets all book count to 0 */
    /** get all books from the folder : database\\books\\ */
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("database\\books\\")) != NULL)/** opens the folder : database\\books\\ */
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)/** reads all the files in there. */
        {
            if(strstr(ent->d_name,".json"))/** checks if the name contains .json to validate if it is a json file */
                load_book_by_filename(ent->d_name);/** pass the file name example : 12345.json */
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        printf("Can`t find directory");
        return 0;
    }
}

/**
* load single file by name from file example : 123456.json
* book file names are ISBN.json
*/
void load_book_by_filename(char* bookName)
{
    int current = total_books_count++;/** gets the place for the new book in the memory */
    Book* newBook= createNewBook(current); /** allocates space for new book in memory and puts it in the array */
    FILE *fileptr;
    char *buffer;/** the array to hold the text in the file */
    long filelen;

    char bookPath[50];
    sprintf(bookPath,"database\\books\\%s",bookName);/** puts the file directory in book path example: 3-598-21500-2.json */

    fileptr = fopen(bookPath, "rb");  // Open the file in binary mode
    if(!fileExists(bookPath))/** validation if the book exists or no. stops if it doesnt find the file */
        return;

    fseek(fileptr, 0, SEEK_END);/** goes to the final character */ // Jump to the end of the file
    filelen = ftell(fileptr);  /** gets the position of the last character */// Get the current byte offset in the file
    rewind(fileptr); /** jumps back to the beginning of the file to use it to read the file.*/// Jump back to the beginning of the file

    buffer = (char *)malloc((filelen+1)*sizeof(char));/** allocates space for all the characters (filelen + 1 because of the null character) */
    fread(buffer, filelen, 1, fileptr);/** reads the file to the end */
    fclose(fileptr);/** closes the reader */

    /**
    Sample JSON FILE FOR BOOK

    {
    "Title":	"Internet & World Wide Web How to Program",
    "ISBN":	"0131752421",
    "Author":	"Jack Sam",
    "Publisher":	"Sayed Ahmed",
    "NoOfCopies":	6,
    "CurrentAvailable":	6,
    "Category":	1,
    "DateOfPublish":	"19-2-1999"
    }
    */
    cJSON *root = cJSON_Parse(buffer);/** puts the whole json file in the root and parses it */
    cJSON *title = cJSON_GetObjectItemCaseSensitive(root, "Title");/** puts the content of Title from the JSON file to *title */
    /** Others are same like title*/
    cJSON *isbn = cJSON_GetObjectItemCaseSensitive(root, "ISBN");
    cJSON *author = cJSON_GetObjectItemCaseSensitive(root, "Author");
    cJSON *publisher = cJSON_GetObjectItemCaseSensitive(root, "Publisher");
    cJSON *noOfCopies = cJSON_GetObjectItemCaseSensitive(root, "NoOfCopies");
    cJSON *CurrentAvailable = cJSON_GetObjectItemCaseSensitive(root, "CurrentAvailable");
    cJSON *bookCategory = cJSON_GetObjectItemCaseSensitive(root, "Category");
    cJSON *dateOfPublish = cJSON_GetObjectItemCaseSensitive(root, "DateOfPublish");


    strcpy(newBook->title,title->valuestring);/** Copies the title read from the JSON file to the newBook Title */
    strcpy(newBook->ISBN,isbn->valuestring);
    strcpy(newBook->author,author->valuestring);
    strcpy(newBook->publisher,publisher->valuestring);

    newBook->date_of_publish = malloc(sizeof(date));/** allocates space for the date*/
    newBook->date_of_publish = date_from_string(dateOfPublish->valuestring);/** converts the date string to date struct */

    newBook->noOfCopies=noOfCopies->valueint;/** copies the no of copies to the newbook->noOfCopies*/
    newBook->currentAvailable=CurrentAvailable->valueint;
    newBook->bookCategory=bookCategory->valueint;
    newBook->needSave = FALSE;/** sets the needsave = 0 because there are no saves needed*/
}
