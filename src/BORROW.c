#include "BORROW.h"
#include "BOOK.h"
#include "MEMBERS.h"
#include "../cJSON.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int is_borrowed_by_this_member(char* ISBN,int Card_Id)
{
    int i;
    for(i=0; i<total_borrows_count; i++)
        if(!strcmp(ISBN,borrows_array[i]->ISBN)
                && Card_Id == borrows_array[i]->CARDID
                && borrows_array[i]->still_borrowed)
            return 1;
    return 0;

}
void borrowTask(int subTask)
{
    switch(subTask)
    {
    case 1:
    {
        borrowBook();
        break;
    }
    case 2:
    {
        returnBook();
        break;
    }
    case 3:
    {
        int i;
        for(i=0; i<total_borrows_count; i++)
            print_borrow(borrows_array[i]);
        break;
    }
    default:
    {
        printf("Unknown borrowing subtask %d\n",subTask);
        break;
    }
    }
}
void borrowBook()
{
    Borrowing* pBorrow = createNewBorrow();
    printf("Enter the book`s ISBN:");
    fflush(stdin);
    gets(pBorrow->ISBN);

    if(!ISBN_USED(pBorrow->ISBN))
    {
        printf("Can`t find a book with this ISBN\n.");
        return;
    }
    int book_Pos= isBook_Available(pBorrow->ISBN);
    if(book_Pos == -1)/** puts the no of available copies in the book_pos
                                                            if it is equal to -1 then no of available copies = 0*/
    {
        printf("There are no available copies for now.\n.");
        return;
    }
    printf("Found book %s\n",books_array[book_Pos]->title);
    printf("Enter the members`s card-id:");
    scanf("%d",&pBorrow->CARDID);

    int member_Pos= find_CardId(pBorrow->CARDID);
    if(member_Pos == -1)
    {
        printf("Can`t find a member with this ID\n.");
        return;
    }

    if(get_books_borrowed(members_array[member_Pos]->Card_Id) == 3)
    {
        printf("This member already borrowed 3 books till now.\n.");
        return;
    }

    if(is_borrowed_by_this_member(pBorrow->ISBN,pBorrow->CARDID))
    {
        printf("This book is already borrowed by this member\n.");
        return;
    }
    pBorrow->date_issued = get_date_now();
    printf("Date of borrow is set to today.\n");
    pBorrow->return_date = get_future_date_from_now(14,0,0);
    printf("Return date is set to 14 days from now %s\n",pBorrow->return_date);
//    scan_date(pBorrow->return_date);

    books_array[book_Pos]->currentAvailable--;// update the book current available copies.
    books_array[book_Pos]->needSave = TRUE;

    pBorrow->still_borrowed = TRUE;
    pBorrow->borrow_UID = total_borrows_count;// set the UID to the current borrow..
    pBorrow->needSave = 1;
    borrows_array[total_borrows_count++] = pBorrow;

    system("@cls");
    print_borrow(pBorrow);
}
void print_borrow(Borrowing* pBorrow)
{
    printf("\n---------------\n");
    printf("Book ISBN:%s\nBorrowing CardId:%d\nBorrowed date:%s\nReturn date:%s"
           ,pBorrow->ISBN
           ,pBorrow->CARDID
           ,date_to_string(pBorrow->date_issued)
           ,date_to_string(pBorrow->return_date));
    printf("\n----------------\n");
}
void load_borrows_from_database()
{
    total_borrows_count= 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("database\\borrows\\")) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            if(strstr(ent->d_name,".json"))
                loadBorrow(ent->d_name);
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
void save_borrows()
{
    printf("\n-------------\n");
    int i,b = 0;
    for(i=0; i<total_borrows_count; i++)
        if(borrows_array[i]->needSave == TRUE)
        {
            saveBorrow(borrows_array[i]);
            b++;
        }
    printf("Saved %d borrows done..\n-------------\n",b);
}
void returnBook()
{
    Borrowing pBorrow;
    fflush(stdin);
    system("@cls");
    printf("Enter the CardID of the member:");
    scanf("%d",&pBorrow.CARDID);
    fflush(stdin);
    printf("Enter the ISBN of the Book:");
    gets(pBorrow.ISBN);

    int book_pos = -1;
    int i,found=FALSE;

    for(i=0; i<total_books_count; i++)
        if(!strcmp(pBorrow.ISBN,books_array[i]->ISBN))
        {
            book_pos = i;
            break;
        }
    if(book_pos == -1)
    {
        printf("Couldn`t find such a book.\n");
        return;
    }

    for(i=0; i<total_borrows_count; i++)
    {
        if(pBorrow.CARDID == borrows_array[i]->CARDID)
            if(!strcmp(pBorrow.ISBN,borrows_array[i]->ISBN))
                if(borrows_array[i]->still_borrowed == TRUE)
                {
                    printf("Found Borrow\n");
                    found=TRUE;
                    break;
                }
    }
    if(!found)
    {
        printf("Couldn`t find such a borrow\n");
        return;
    }

    print_borrow(borrows_array[i]);
    if(cmpdates(borrows_array[i]->return_date,get_date_now()) != -1)// return date > current date
        printf("Book is returned before the return date.\n");
    else
        printf("Book is returned but the return date is late.\n");

    books_array[book_pos]->currentAvailable++;// book returned.
    books_array[book_pos]->needSave = TRUE;

    borrows_array[i]->actual_return = get_date_now();
    borrows_array[i]->still_borrowed = FALSE;
    borrows_array[i]->needSave = TRUE;
}
void saveBorrow(Borrowing *pBorrow)
{
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ISBN", cJSON_CreateString(pBorrow->ISBN));
    cJSON_AddItemToObject(root, "CARDID", cJSON_CreateNumber(pBorrow->CARDID));
    cJSON_AddItemToObject(root, "date_issued", cJSON_CreateString(date_to_string(pBorrow->date_issued)));
    cJSON_AddItemToObject(root, "return_date", cJSON_CreateString(date_to_string(pBorrow->return_date)));

    if(!pBorrow->still_borrowed)
        cJSON_AddItemToObject(root, "actual_return", cJSON_CreateString(date_to_string(pBorrow->actual_return)));

    cJSON_AddItemToObject(root,"borrow_UID",cJSON_CreateNumber(pBorrow->borrow_UID));
    cJSON_AddItemToObject(root,"still_borrowed",cJSON_CreateNumber(pBorrow->still_borrowed));
    FILE* fptr;
    char fileName[30] ="database\\borrows\\";
    sprintf(fileName,"database\\borrows\\%d.json",pBorrow->borrow_UID);
    char *jsonFile = cJSON_Print(root);
    fptr = fopen(fileName,"w");
    fprintf(fptr,jsonFile);
    fclose(fptr);
    pBorrow->needSave = FALSE;
    printf("Borrows of BOOK: %s UID: %d updated successfully.\n",pBorrow->ISBN,pBorrow->borrow_UID);
}
void loadBorrow(char* fileName)
{
    int current = total_borrows_count++;
    // realloc(myBooks,sizeof(Book)*totalBooks);
    Borrowing* newBorrow= createNewBorrow(current);
    FILE *fileptr;
    char *buffer;
    long filelen;
    char borrow_Path[50];
    sprintf(borrow_Path,"database\\borrows\\%s",fileName);

    fileptr = fopen(borrow_Path, "rb");  // Open the file in binary mode
    if(!fileExists(borrow_Path)) return;
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    cJSON *root = cJSON_Parse(buffer);

    cJSON *ISBN = cJSON_GetObjectItemCaseSensitive(root, "ISBN");
    cJSON *CARDID = cJSON_GetObjectItemCaseSensitive(root, "CARDID");
    cJSON *STILL_BORROWED = cJSON_GetObjectItemCaseSensitive(root, "still_borrowed");
    cJSON *BORROW_UID = cJSON_GetObjectItemCaseSensitive(root, "borrow_UID");
    cJSON *DATE_ISSUED = cJSON_GetObjectItemCaseSensitive(root, "date_issued");
    cJSON *RETURN_DATE = cJSON_GetObjectItemCaseSensitive(root, "return_date");
    cJSON *ACTUAL_DATE = cJSON_GetObjectItemCaseSensitive(root, "actual_return");

    strcpy(newBorrow->ISBN,ISBN->valuestring);
    newBorrow->CARDID = CARDID->valueint;
    copy_date(newBorrow->date_issued,date_from_string(DATE_ISSUED->valuestring));
    copy_date(newBorrow->return_date,date_from_string(RETURN_DATE->valuestring));
    newBorrow->borrow_UID = BORROW_UID->valueint;
    newBorrow->still_borrowed = STILL_BORROWED->valueint;

    if(!STILL_BORROWED->valueint)
    {
        if(ACTUAL_DATE != NULL)
            copy_date(newBorrow->actual_return,date_from_string(ACTUAL_DATE->valuestring));
        else
            printf("Invalid book borrow skipped of ID :%d\n",BORROW_UID->valueint);
    }

    newBorrow->needSave = FALSE;
    borrows_array[current]=newBorrow;
}
Borrowing* createNewBorrow()
{
    Borrowing* ptr = (Borrowing*)malloc(sizeof(Borrowing));
    ptr->actual_return  = malloc(sizeof(date));
    ptr->date_issued  = malloc(sizeof(date));
    ptr->return_date  = malloc(sizeof(date));
    return ptr;
}
