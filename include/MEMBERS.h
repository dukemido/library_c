#include "BOOK.h"
#include "BORROW.h"

typedef struct
{
    char firstName[20], lastName[20],address[50], phoneNumber[12],emailAddress[50];
    int Card_Id;
    int age;
    int needSave;
} Member;
int get_books_borrowed(int);
void memberTask(int);
int find_CardId(int);
Member* createNewMember(int);

Member* members_array[1000];
Member* deleted_members_array[10000];

void load_members_from_database();
void load_member_from_file(char*);
int total_members_count,total_members_deleted_count;

void memberTask(int);
void save_member(Member*);
void delete_member(Member*);
void delete_member_menu();
void print_member(Member*);
