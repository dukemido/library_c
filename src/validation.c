#include "validation.h"

/**
    Validates if the string has number
*/
int contains_number(char *str)
{
    if(strstr(str,"0")||
            strstr(str,"1")||
            strstr(str,"2")||
            strstr(str,"3")||
            strstr(str,"4")||
            strstr(str,"5")||
            strstr(str,"6")||
            strstr(str,"7")||
            strstr(str,"8")||
            strstr(str,"9"))
        return 1;

    return 0;

}

/**
    Validates email by checking for the .com and @
*/
int is_valid_email(char* email)
{
    if(!strstr(email,"@"))
        return 0;
    if(!strstr(email,".com"))
        return 0;

    return 1;
    /** must be returned with 1 to be valid
    * if didn`t find the @ or the .com will return 0
    */
}

/**
    This is a 3 factor validation.
    1st validates that the number is 11 numbers.
    2nd validates if the phone number all characters are numbers.
    3rd validates if the first number is zero w second number is 1 and third number is 0,1,2,5
*/
int is_valid_phone_number(char* number)
{
    int n = strlen(number);
    if(n != 11)
        return 0;/** checks if the number is 11 characters */
    int i = 0;
    n =0;
    for(i=0; i<strlen(number); i++)/** checks if all the characters are numbers (no alphabets or special characters) */
    {
        char current_number = number[i];
        if(current_number == '0' ||
                current_number == '1' ||
                current_number == '2' ||
                current_number == '3' ||
                current_number == '4' ||
                current_number == '5' ||
                current_number == '6' ||
                current_number == '7' ||
                current_number == '8' ||
                current_number == '9')
            n++;
    }
    if(n!=11)/** are all the 11 characters numbers?*/
        return 0;

    /** validates that the first number is 0 second is 1 and the third number is vf,mobinil,etisilat w we :P
        7rka zyada kda :P
        */
    if(strlen(number) > 3)

        if(number[0] == '0' && number[1] == '1'
                && (number[2] == '1' || number[2] == '2' || number[2] == '0' || number[2] == '5'))
            return 1;
    return 0;
}

/**
    Returns 0 if spaces are found in the word
    Returns 1 if there are no spaces found.
*/
int is_valid_single_char(char* word)
{
    int i;
    for(i=0; i<strlen(word); i++)/** loops over all the characters and checks if it contains space*/
        if(word[i] == ' ')
            return 0;
    return 1;
}
