#include "contact.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "ctype.h"

short exitFlag = 0;

short loopRunner = 1;
/*********************************PRINTING HORIZONTAL LINE*******************************/
void print_line()
{
    printf("%15s"," ");
    for(int i = 0; i < 50; i++)
    {
        printf("-");
    }
}
/***************************************************************************************/
/**********************************LOADING TEXT*****************************************/
void loading(int usage)
{
    char *string_print1 = malloc(25 * sizeof(char));

    char *string_print2 = malloc(25 * sizeof(char));
    printf("\n");
    if(!(usage ^ 1))
    {
        strcpy(string_print1,"Loading Contacts[");
        strcpy(string_print2,"Contacts Loaded");
    }
    else if(!usage)
    {    
        strcpy(string_print1,"Redirecting MainPage[");
        strcpy(string_print2,"MainPage redirected");
    }
    else if(!(usage ^ 3))
    {
        strcpy(string_print1,"Redirecting EditingPage[");
        strcpy(string_print2,"EditingPage redirected");
    }
    else //2
    {
        strcpy(string_print1,"Saving Contacts[");
        strcpy(string_print2,"Contacts saved");
    }

    
    for(int i = 1; i <= 100; i+=2)
    {
        printf("%15s%s"," ",string_print1);
        for(int j = 1; j <= 100; j+=2)
        {
            if(j <= i)  printf("\033[1;44m-\033[0m");

            else printf(" ");    
        }
        printf("]%d%%",i+1);
        fflush(stdout);
        usleep(50000);
        printf("\r");
    }
    printf("\n%15s%s\n\n"," ",string_print2);

    free_up_str(string_print1,string_print2);
}
/***************************************************************************************/
/**********************FUNCTION TO GET VALID INPUT FROM USER****************************/
int invalid_input(int iter)
{
    while(iter)
    {
        printf("\n%15sPlease Enter correct option (1 - Continue) / (0 - Exit) : ", " ");
        scanf("%d",&iter);

        if((!(iter ^ 1)) || !iter)  break;
    }
    return iter;
}
/***************************************************************************************/
/**********************************MAIN FUNCTION****************************************/
int main()
{
    AddressBook *contacts = malloc(sizeof(Contact)); 

    FILE *info;
    contacts->contactCount = 0;
    loadContact(contacts, info);

    printf("%59s\033[1;44m%s\033[0m\n\n", " ", " ADDRESSBOOK ");

    while(loopRunner)
    {
        
        printf("\n%25s"," ");
        print_line();
        printf("\n%41s%49s\n%41s%21s\033[1;30;47m%s\033[0m%22s\n%41s%49s\n%25s","|","|","|"," "," MENU ","|","|","|"," ");
        print_line();

        printf("\n%41s%49s\n%41s%11s%21s%17s\n%41s%49s\n%41s%11s%21s%17s\n%41s%49s","|","|","|"," ","1.CREATE CONTACT","|","|","|","|"," ","2.SEARCH CONTACT","|","|","|");
        
        printf("\n%41s%11s%23s%15s\n%41s%49s\n%41s%11s%19s%19s\n%41s%49s","|"," ","3.LIST ALL CONTACT","|","|","|","|"," ","4.EDIT CONTACT","|","|","|");

        printf("\n%41s%11s%21s%17s\n%41s%49s\n%41s%11s%11s%27s\n%41s%49s\n%25s","|"," ","5.DELETE CONTACT","|","|","|","|"," ","6.EXIT","|","|","|"," ");
        print_line();

        printf("\n\n%15s\033[1mPlease enter an option : \033[0m"," ");
        int opt = 0;

        start:
        scanf("%d",&opt);
        switch(opt)
        {
            case 1:
                createContact(contacts, info);
                break;

            case 2:
                searchContact(contacts, info);
                loopRunner = 1;
                break;

            case 3:
                listContacts(contacts, info);
                break;

            case 4:
                editContact(contacts, info);
                loopRunner = 1;
                break;

            case 5:
                deleteContact(contacts, info); 
                loopRunner = 1;
                break;

            case 6:
                printf("\n%15s\033[1;41m:::!!!EXIT!!!:::\033[0m\n"," ");
                printf("\n\n%15s\033[1;37;43m!ARE YOU SURE!\033[0m (1 - Yes) / (0 - No) : ", " ");
                exitFlag = 1;
                scanf("%hd",&loopRunner);
                if(!loopRunner)  loopRunner = 1;
                else if(loopRunner ^ 1 && loopRunner ^ 0)     loopRunner = invalid_input(loopRunner);
                else if(loopRunner ^ 0)  loopRunner = !loopRunner;
                break;

            default:
                printf("\n%15s\033[1;31;47m:::Invalid Input:::\033[0m\n\n%15s\033[1mPlease enter a correct option\033[0m (Ex : 2) : "," "," ");
                goto start;
        }
    }
    exit:
    printf("\n%82s\n","\033[1;44m THANK YOU \033[0m");
    printf("\n");
    return 0;
}
/***************************************************************************************/