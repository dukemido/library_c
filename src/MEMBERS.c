#include "MEMBERS.h"
#include "BOOK.h"
#include "../cJSON.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int find_CardId(int Card_Id)
{
    int i,found=-1;
    for(i =0; i<total_members_count; i++)
    {
        if(Card_Id == members_array[i]->Card_Id)
        {
            found = i;
            break;
        }
    }
    return found;
}
int get_books_borrowed(int Card_ID)
{
    int i,count = 0;
    for(i=0; i<total_borrows_count; i++)
        if(Card_ID == borrows_array[i]->CARDID)
            if(borrows_array[i]->still_borrowed)
                count++;
    return count;
}
Member* createNewMember(int place)
{
    Member* ptr = (Member*)malloc(sizeof(Member));
    members_array[place]= ptr;
    return ptr;
}
void load_members_from_database()
{
    total_members_count = total_members_deleted_count= 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("database\\members\\")) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            if(strstr(ent->d_name,".json"))
                load_member_from_file(ent->d_name);
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
void load_member_from_file(char* memberName)
{
    int current = total_members_count++;
    Member* newMember= createNewMember(current);
    FILE *fileptr;
    char *buffer;
    long filelen;

    char bookPath[50];
    sprintf(bookPath,"database\\members\\%s",memberName);

    fileptr = fopen(bookPath, "rb");  // Open the file in binary mode
    if(!fileExists(bookPath)) return;
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    cJSON *root = cJSON_Parse(buffer);
    cJSON *firstName = cJSON_GetObjectItemCaseSensitive(root, "FirstName");
    cJSON *lastName = cJSON_GetObjectItemCaseSensitive(root, "LastName");
    cJSON *cardID = cJSON_GetObjectItemCaseSensitive(root, "CardID");
    cJSON *address = cJSON_GetObjectItemCaseSensitive(root, "Address");
    cJSON *phoneNumber = cJSON_GetObjectItemCaseSensitive(root, "PhoneNumber");
    cJSON *emailAddress = cJSON_GetObjectItemCaseSensitive(root, "Email");
    cJSON *age = cJSON_GetObjectItemCaseSensitive(root, "Age");

    strcpy(newMember->firstName,firstName->valuestring);
    strcpy(newMember->lastName,lastName->valuestring);
    strcpy(newMember->address,address->valuestring);
    strcpy(newMember->phoneNumber,phoneNumber->valuestring);
    strcpy(newMember->emailAddress,emailAddress->valuestring);

    newMember->Card_Id = cardID->valueint;
    newMember->age=age->valueint;
    newMember->needSave = FALSE;
}
void memberTask(int subTask)
{
    switch(subTask)
    {
    case 1:
    {
        insert_new_member();
        break;
    }
    case 2:
    {
        delete_member_menu();
        break;
    }
    case 3:
    {
        int i;
        for(i=0; i<total_members_count; i++)
            print_member(members_array[i]);
        break;
    }
    default:
    {
        printf("Unknown member subtask %d\n",subTask);
        break;
    }
    }
}
void save_delete_members()
{
    int s = 0,d = 0;
    printf("\n-------------\n");
    int i;
    for(i=0; i<total_members_count; i++)
        if(members_array[i]->needSave)
        {
            save_member(members_array[i]);
            s++;
        }
    printf("Saved %d members done..\n-------------\n",s);
    for(i=0; i<total_members_deleted_count; i++)
    {
        delete_member(deleted_members_array[i]);
        d++;
    }
    total_members_deleted_count = 0;
    printf("Deleted %d members done..\n-------------\n",d);


}
void insert_new_member()
{
    system("@cls");
    fflush(stdin);
    Member* newMember = (Member*)malloc(sizeof(Member));

    do
    {
        printf("First Name (Example:Ahmed) :");
        gets(newMember->firstName);
    }
    while(!is_valid_single_char(newMember->firstName));

    do
    {
        printf("Last Name (Example:Muhammad) :");
        gets(newMember->lastName);
    }
    while(!is_valid_single_char(newMember->lastName));
    int trials = 0;
    do
    {
        if(trials > 0)
            printf("This member already exists choose another card id.\n");
        do
        {
            printf("Card ID (1 to 10000):");
            scanf("%d",&newMember->Card_Id);
        }
        while(newMember->Card_Id < 1 || newMember->Card_Id > 10000);
        trials++;
    }
    while(find_CardId(newMember->Card_Id) != -1);

    fflush(stdin);

    trials = 0;
    do
    {
        if(trials > 0)
            printf("Address must contain street number.\n");
        do
        {
            printf("Address (st number, street and city):");
            gets(newMember->address);
        }
        while(strlen(newMember->address) == 0);
        trials++;
    }
    while(!contains_number(newMember->address));

    do
    {
        printf("Enter a valid Phone.No:");
        gets(newMember->phoneNumber);
    }
    while(is_valid_phone_number(newMember->phoneNumber)!= 1);

    do
    {
        printf("Enter a valid Email-Address (Example: example@gmail.com):");
        gets(newMember->emailAddress);
    }
    while(is_valid_email(newMember->emailAddress)!= 1);

    do
    {
        printf("Age (10 to 100) :");
        scanf("%d",&newMember->age);
    }
    while(newMember->age < 10 || newMember->age > 100);

    print_member(newMember);
    newMember->needSave = 1;

    members_array[total_members_count++] = newMember;
}
void save_member(Member* newMember)
{
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "FirstName", cJSON_CreateString(newMember->firstName));
    cJSON_AddItemToObject(root, "LastName", cJSON_CreateString(newMember->lastName));
    cJSON_AddItemToObject(root, "CardID", cJSON_CreateNumber(newMember->Card_Id));
    cJSON_AddItemToObject(root, "Address", cJSON_CreateString(newMember->address));
    cJSON_AddItemToObject(root, "PhoneNumber", cJSON_CreateString(newMember->phoneNumber));
    cJSON_AddItemToObject(root, "Email", cJSON_CreateString(newMember->emailAddress));
    cJSON_AddItemToObject(root,"Age",cJSON_CreateNumber(newMember->age));

    FILE* fptr;
    char *jsonFile = cJSON_Print(root);

    char fileName[30];
    sprintf(fileName,"database\\members\\%d.json",newMember->Card_Id);

    fptr = fopen(fileName,"w");
    fprintf(fptr,jsonFile);
    fclose(fptr);
    newMember->needSave = 0;
    printf("Member %d updated successfully.\n",newMember->Card_Id);
}
void print_member(Member* myMember)
{
    printf("\n---------------\n");
    printf("First Name:%s\nLast Name:%s\nCardID:%d\nHome address:%s\nPhone number:%s\nEmail:%s\nAge:%d\n---------\n",myMember->firstName
           ,myMember->lastName
           ,myMember->Card_Id
           ,myMember->address
           ,myMember->phoneNumber
           ,myMember->emailAddress
           ,myMember->age);
    //TODO: Add the books borrowed to be displayed.
}
void delete_member(Member* myMember)
{
    char path[100];
    sprintf(path,"database\\members\\%d.json",myMember->Card_Id);

    if(!remove(path))
        printf("Deleted member of ID -->%d\n",myMember->Card_Id);
}
void delete_member_menu()
{
    printf("Enter your member Card_Id:");
    int Card_Id;
    fflush(stdin);
    scanf("%d",&Card_Id);

    int i,memberpos = -1;
    for(i = 0; i<total_members_count; i++)
        if(Card_Id == members_array[i]->Card_Id)
        {
            memberpos = i;
            break;
        }

    if(memberpos == -1)
    {
        printf("No such CardId for a member.\n");
        return;
    }
    int borrowed_books_count = get_books_borrowed(Card_Id);
    if(borrowed_books_count > 0)
    {
        printf("This member borrowed %d books can`t delete him till he returns them.\n",borrowed_books_count);
        return;
    }
    fflush(stdin);
    printf("Are you sure you want to delete the member \"%s\" (y/n)?\n",members_array[memberpos]->firstName);
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

    deleted_members_array[total_members_deleted_count++] = members_array[memberpos];
    members_array[memberpos] = members_array[--total_members_count];
    printf("Deleted the member successfully..\n");
}

