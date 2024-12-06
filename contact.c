#include "contact.h"
#include "stdlib.h"
//LINKING VARIABLES
extern short exitFlag;
extern short loopRunner;
/******************VALIDATION, GET CONTACT, EDIT FUNCTIONS DECLATATION***********************/   
int getContact(AddressBook *addressBook, FILE *fp, int option, int *serial_no);
int name_validate(char []);
int phone_number_validate(char []);
int email_validate(char []);
int editingPage(int option, int function_no);
void editing_contactDetails(AddressBook *addressBook,int serial_no, int option, FILE *fp);
/*********************************************************************************************/
/******************************PRINTF FUNCTIONS DECLARATION***********************************/
void print_limit_reached()
{   printf("\n%15s\033[1;37;41m:::!!!RE-ENTERING NAME LIMIT REACHED!!!:::\033[0m\n\n%15s\033[1mTry again :-)\033[0m\n"," ", " ");   }
void print_invalid_name()
{   printf("\n%15s\033[1;37;41m:::!!!INVALID INPUT!!!:::\033[0m\n\n%15s\033[1mName must contains only alphabets / underscores\033[0m (Ex : Bala_N)\n", " ", " ");   }
void print_invalid_phone_number()
{   printf("\n%15s\033[1;37;41m:::!!!INVALID INPUT!!!:::\033[0m\n\n%15s\033[1mPhone Number must contains only numbers with 10 digits\033[0m (Ex : 9876543210)\n", " ", " ");    }
void print_invalid_email()
{   printf("\n%15s\033[1;37;41m:::!!!INVALID INPUT!!!:::\033[0m\n\n%15s\033[1mEmail ID should be in proper naming(i.e name@domain_name.com)\033[0m (Ex : ronaldo7@gmail.com)\n", " ", " "); }
void print_contact_found()
{
    print_line();   printf("\n%15s\033[1;42m:::CONTACT FOUND:::\033[0m\n"," ");
    print_line();   printf("\n");
}
/*********************************************************************************************/
void initialize(AddressBook *addressBook)
{
    addressBook->contacts = malloc(sizeof(Contact) * 100);
}
void free_up_str(char *str1, char *str2)
{
    free(str1);
    free(str2);
}
short re_entry_print(int *invalid_count)
{
    (*invalid_count)++;
    if((*invalid_count) >= 6)
    {
            print_limit_reached();
            loading(0);
            return 0;
    }
    printf("\n%15sRetry limits %d\n", " ",(6 - (*invalid_count)));
    return 1;
}
/**********************************************************************************************/
/*************************************LIST CONTACTS********************************************/
void listContacts(AddressBook *addressBook, FILE *fp) 
{
    for(int i = 0; i < addressBook->contactCount;i++)
    {
        print_line();
        printf("\n\n%15s\033[1;46mContact %d \033[0m\n%15sName  : %s\n%15sPhone : %s\n%15sEmail : %s\n\n", " ", (i + 1), " ", addressBook->contacts[i].name, " ", addressBook->contacts[i].phone, " ", addressBook->contacts[i].email);
    }
    print_line();
    printf("\n%15s\033[1;42m:::CONTACTS LISTED:::\033[0m\n"," ");
    print_line();
    loading(0);
}
/***********************************************************************************************/
/*************************************CREATE CONTACTS********************************************/
void createContact(AddressBook *addressBook, FILE *fp) 
{
    short res = 0;
    int invalid_count = 0;
    read_name:
    getchar();
    printf("\n\n%15s\033[1mEnter the Contact Name : \033[0m", " ");
    scanf("%[^\n]",addressBook->contacts[addressBook->contactCount].name);

    exitFlag =  name_validate(addressBook->contacts[addressBook->contactCount].name);
    if(!exitFlag)
    {   
        print_invalid_name();
        res = re_entry_print(&invalid_count);
        if(res) goto read_name;
        else return;
    }

    invalid_count = 0;
    read_phone_number:
    getchar();
    printf("\n\n%15s\033[1mEnter the Contact Phone Number : \033[0m", " ");
    scanf("%[^\n]",addressBook->contacts[addressBook->contactCount].phone);

    exitFlag =  phone_number_validate(addressBook->contacts[addressBook->contactCount].phone);
    //CHECK WITH REMAINING PHONE NUMBERS
    if(exitFlag)
    {
        int i = 0;
        while(i < addressBook->contactCount)
        {
            if(!(strcmp(addressBook->contacts[addressBook->contactCount].phone, addressBook->contacts[i].phone)))
            {
                printf("\n%15s\033[1;37;41m:::!!!Entered Phone Number already exist in another contact!!!:::\033[0m\n\n%15s\033[1mTry again :-)\033[0m \n"," ", " ");
                goto re_enter_limit_3;
            }
            i++;
        }
    }
    if(!exitFlag)
    {   
        print_invalid_phone_number();
            re_enter_limit_3:
        res = re_entry_print(&invalid_count);
        if(res) goto read_phone_number;
        else return;
    }

    invalid_count = 0;
    read_email:
    getchar();
    printf("\n\n%15s\033[1mEnter the Contact Email : \033[0m", " ");
    scanf("%[^\n]",addressBook->contacts[addressBook->contactCount].email);

    exitFlag =  email_validate(addressBook->contacts[addressBook->contactCount].email);
    //CHECK WITH REMAINING EMAIL IDS
    if(exitFlag)
    {
        int i = 0;
        while(i < addressBook->contactCount)
        {
            if(!(strcmp(addressBook->contacts[addressBook->contactCount].email, addressBook->contacts[i].email)))
            {
                printf("\n%15s\033[1;37;41m:::!!!Entered Email ID already exist in another contact!!!:::\033[0m\n\n%15s\033[1mTry again :-)\033[0m \n", " ", " ");
                goto re_enter_limit_4;
            }
            i++;
        }  
    }
    if(!exitFlag)
    {  
        print_invalid_email(); 
            re_enter_limit_4:
        res = re_entry_print(&invalid_count);
        if(res) goto read_email;
        else return;
    }

    fp = fopen("AddressBook.csv","w");
    rewind(fp);
    fprintf(fp,"#%d#\n",(addressBook->contactCount+1));
    for(int i = 0; i <= addressBook->contactCount;i++)
        fprintf(fp,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);

    fclose(fp);

    addressBook->contactCount++;

    print_line();
    printf("\n%15s\033[1;42m:::CONTACTS CREATED:::\033[0m\n"," ");
    print_line();
    loading(0);
}
/************************************************************************************************************************/
/************************************************SEARCH CONTACTS*********************************************************/
void searchContact(AddressBook *addressBook, FILE *fp) 
{
    int serial_no = 0;
    while(loopRunner)
    {
        loopRunner = getContact(addressBook, fp, 2, &serial_no);
        //if loopRunner == 0 terminate the function/redirect to main page
        if(!loopRunner) 
        {   //Redircting...
            loading(0);
            return;
        }

        if(!exitFlag)   continue;

        if(!(loopRunner ^ 2))
        {
            loopRunner = 1;
            continue;
        }
        //PRINT THE CONTACT   
        printf("\n\n%15s\033[1;46mContact %d \033[0m\n%15sName  : %s\n%15sPhone : %s\n%15sEmail : %s\n\n", " ", (serial_no + 1), " ", addressBook->contacts[serial_no].name, " ", addressBook->contacts[serial_no].phone, " ", addressBook->contacts[serial_no].email);
        print_line();
    }
}
/************************************************************************************************************************/
/************************************************EDIT CONTACTS***********************************************************/
void editContact(AddressBook *addressBook, FILE *fp) 
{
    int serial_no = 0;
    while(loopRunner)
    {
        loopRunner = getContact(addressBook, fp, 4, &serial_no);

        if(!loopRunner) 
        {   //Redircting...
            loading(0);
            return;
        }

        if(!exitFlag)   continue;
        printf("\n\n%15s\033[1;46mContact %d \033[0m\n%15sName  : %s\n%15sPhone : %s\n%15sEmail : %s\n\n", " ", (serial_no + 1), " ", addressBook->contacts[serial_no].name, " ", addressBook->contacts[serial_no].phone, " ", addressBook->contacts[serial_no].email);
        print_line();
        //MODIFY
        loading(3);//Redirecting editing page 
        editing_contactDetails(addressBook, serial_no, 4 , fp);
    }
}
/************************************************************************************************************************/
/************************************************DELETE CONTACTS*********************************************************/
void deleteContact(AddressBook *addressBook, FILE *fp) 
{
    int serial_no = 0;
    while(loopRunner)
    {
        loopRunner = getContact(addressBook, fp, 5, &serial_no);
        //if loopRunner == 0 terminate the function/redirect to main page
        if(!loopRunner) 
        {   //Redircting...
            loading(0);
            return;
        }

        if(!exitFlag)   continue;
        //DELETE THE CONTACT
           //PRINT THE CONTACT
        printf("\n\n%15s\033[1;46mContact %d \033[0m\n%15sName  : %s\n%15sPhone : %s\n%15sEmail : %s\n\n", " ", (serial_no + 1), " ", addressBook->contacts[serial_no].name, " ", addressBook->contacts[serial_no].phone, " ", addressBook->contacts[serial_no].email);
        print_line();
           //ASK USER CONFIRMATION
        printf("\n\n%15s\033[1;37;43m!ARE YOU SURE YOU WANT TO DELETE!\033[0m (1 - Yes) / (0 - No) : ", " ");
        int opt = userConfirmation(0);
        printf("\n\n");
        print_line();
        if(!opt)    
        {
            loading(0);
            return;
        }

        struct SAMPLE_CONTACTS *samplecontacts= malloc(sizeof(struct SAMPLE_CONTACTS) * (addressBook->contactCount - 1));
        if (!samplecontacts) {
            printf("Memory allocation failed!\n");
            return;
        }
        int i = 0, j = 0;
        while(i < addressBook->contactCount)
        {  
            if(!(serial_no ^ i))
            {
                i++;
                continue;
            }    
            else
            {
                strcpy(samplecontacts[j].sample_name, addressBook->contacts[i].name);
                strcpy(samplecontacts[j].sample_phone, addressBook->contacts[i].phone);
                strcpy(samplecontacts[j].sample_email, addressBook->contacts[i].email);
                j++;
            }
            i++; 
        }
        addressBook->contactCount--;
        
        printf("\n%15s\033[1;42m:::CONTACT DELETED:::\033[0m\n", " ");
        print_line();

        saveContact(samplecontacts, fp, addressBook->contactCount);
        free(addressBook -> contacts);
        free(samplecontacts);
        loadContact(addressBook, fp);
    }
}
/************************************************************************************************************************/
/**************************************************LOAD CONTACTS*********************************************************/
void loadContact(AddressBook *addressBook, FILE *fp)
{
    fp= fopen("AddressBook.csv", "r");
    rewind(fp);
    
    if (fp == NULL) 
    {
        printf("Error opening file.\n");
        return;
    }
    if (addressBook == NULL) {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return;
    }
    initialize(addressBook);
    fscanf(fp,"#%d#\n",&addressBook->contactCount);
    for(int i = 0; i < addressBook->contactCount; i++)
    { 
        fscanf(fp, " %[^,],%[^,],%[^\n]\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
        if(feof(fp))   break;
    }
    fclose(fp);
}
/************************************************************************************************************************/
/**************************************************SAVE CONTACTS*********************************************************/
void saveContact(struct SAMPLE_CONTACTS *samplecontacts, FILE *fp, int countContact)
{
    fp = fopen("AddressBook.csv","w+");
    rewind(fp);

    fprintf(fp,"#%d#\n",countContact);
    for(int i = 0; i < countContact; i++)
    { 
        if(feof(fp))    break;
        fprintf(fp,"%s,%s,%s\n",samplecontacts[i].sample_name,samplecontacts[i].sample_phone,samplecontacts[i].sample_email);
    }
    fclose(fp);
}
/************************************************************************************************************************/
/*********************************************************GET CONTACT FROM USER***********************************************************/
                                                /*USED FOR SEARCH, EDIT AND DELETE OPTION*/
int getContact(AddressBook *addressBook, FILE *fp, int option, int *serial_no)
{
    char *heading = malloc(20 * sizeof(char));
    
    if(option == 2)         strcpy(heading,"SEARCH CONTACT");
    else if(option == 4)    strcpy(heading," EDIT CONTACT ");
    else if(option == 5)    strcpy(heading,"DELETE CONTACT");
    
    //INITIALIZING STRINGS
        int opt = 0;
        char *search_name = malloc(sizeof(char) * 50);
        int name_count = 0;
        char *search_phone = malloc(sizeof(char) * 20);

        //TABLE
        printf("\n%25s"," ");
        print_line();
        printf("\n%41s%49s\n%41s%16s\033[1;37;45m %s \033[0m%17s\n%41s%49s\n%25s", "|", "|", "|", " ", heading, "|", "|", "|", " ");
        print_line();

        printf("\n%41s%49s\n%41s%7s%21s%21s\n%41s%49s\n%41s%11s%25s%13s\n%41s%49s\n%41s%10s%11s%28s\n%41s%49s\n%25s", "|", "|", "|", " ", "1.SEARCH NAME", "|", "|", "|", "|", " ", "2.SEARCH PHONE NUMBER", "|", "|", "|", "|", " ", "3.EXIT", "|", "|", "|", " ");
        print_line();
        printf("\n");

        free(heading);
        //ASKING USER TO ENTER AN OPTION
        printf("\n\n%15s\033[1mPlease enter an option : \033[0m", " ");
        start:
        scanf("%d",&opt);

        switch(opt)
        {
            case 1: goto search_by_name;

            case 2: goto search_by_phone;
        
            case 3:
                free_up_str(search_name,search_phone);
                return 0;

            default:
                printf("\n%15s\033[1;37;41m:::!!!INVALID INPUT!!!:::\033[0m\n\n%15s\033[1mPlease enter a correct option\033[0m (Ex : 2) : ", " ", " ");
                goto start;
        }

        //search by name
        search_by_name:
        printf("\n%15sEnter name to search a contact : ", " ");
        scanf(" %[^\n]",search_name);
        exitFlag =  name_validate(search_name);
        if(!exitFlag)
        {   
            print_invalid_name();
            free_up_str(search_name,search_phone);
            return 1;
        }

        for(int i = 0; i < addressBook->contactCount;i++)
        {
            if(!strcmp(search_name,addressBook->contacts[i].name))
            {
                name_count++;
                *serial_no = i;
            }     
        }

        if(name_count > 1)  
        {
            printf("\n%15s\033[1;37;41m %d \033[0m contacts have same name \033[1m'%s'\033[0m\n\n%15sDo you want to search by phone number (1 - Yes) / (0 - No): ", " ", name_count, search_name, " ");
            getchar();
            *serial_no = name_count = 0;
            opt = userConfirmation(0);
            if(!opt)
            {
                free_up_str(search_name,search_phone);
                return 0;
            }
            else    goto search_by_phone;
        }
        else if(!name_count)    
        {
            printf("\n%15s\033[1;41m:::!!!CONTACT NOT FOUND!!!:::\033[0m\n\n%15sDo you want to search by phone number (1 - Yes) / (0 - No): "," ", " ");
            opt = userConfirmation(0);
            if(!opt)
            {
                free_up_str(search_name,search_phone);
                return 0;
            }
            else    goto search_by_phone;
        }
        else    //CONTACT FOUND
        {
            print_contact_found();
            free_up_str(search_name,search_phone);
            return 1;
        }

        //search by phone
        search_by_phone:
        printf("\n%15sEnter phone number to search a contact : ", " ");
        scanf(" %[^\n]",search_phone);
        exitFlag = phone_number_validate(search_phone);

        if(!exitFlag)
        {
            print_invalid_phone_number();
            free_up_str(search_name,search_phone);
            return 1;
        }

        for(int i = 0; i < addressBook->contactCount;i++)
        {
            if(!strcmp(search_phone,addressBook->contacts[i].phone))
            {
                name_count++;
                *serial_no = i;
                break;
            }
        }
        if(!name_count) 
        {
            printf("\n%15s\033[1;41m:::!!!CONTACT NOT FOUND!!!:::\033[0m\n", " ");
            return 2;
        }
        else    //CONTACT FOUND
        {
            print_contact_found();
            free_up_str(search_name,search_phone);
            return 1;
        }     
}
/************************************************************************************************************************/
/***********************************************VALIDATION FUNCTIONS DEFINTION*****************************************************/
int name_validate(char str[])//NAME VALIDATION
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        if((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] == '_'))
            continue;
        else return 0;
    }
    return 1;
}

int phone_number_validate(char str[])//PHONE NUMBER VALIDATION
{
    if(strlen(str) != 10)   return 0;

    for(int i = 0; str[i] != '\0'; i++)
    {
        if(str[i] >= '0' && str[i] <= '9')
            continue;
        else return 0;
    }
    return 1;
}

int email_validate(char *str)//EMAIL VALIDATION
{
    //SEARCH FOR ".com"
    if(!(strstr(str,".com")))   return 0;

    int length =strlen(str), flag_at = 0;
    
    //CHECK LAST FOUR CHARATERS AS '.com'
    if(!(!(str[length - 1] ^ 'm')) && (!(str[length - 2] ^ 'o')) && (!(str[length - 3] ^ 'c')) && (!(str[length - 4] ^ '.')))   return 0;
    //CHECK THE FIRST  AND LAST CHARACTER (SPECIAL CHARACTERS ARE NOT ALLOWED AT STARTING AND ENDING OF THE EMAIL ID)
    if(!((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z')))   return 0;
 
    if(!((str[length - 1] >= 'A' && str[length - 1] <= 'Z') || (str[length - 1] >= 'a' && str[length - 1] <= 'z')))   return 0;

    for(int i = 0; str[i] != '\0'; i++)
    {
        //SEARCH FOR '@'
        if(!(str[i] ^ '@'))
        {
            flag_at++;//CHECK '@' 
           //CHECK THE ADJACENT CHARACTER
            if(!((str[i - 1] >= 'A' && str[i - 1] <= 'Z') || (str[i - 1] >= 'a' && str[i - 1] <= 'z') || (str[i - 1] >= '0' && str[i - 1] <= '9'))) 
                return 0;
            
            if(!((str[i + 1] >= 'A' && str[i + 1] <= 'Z') || (str[i + 1] >= 'a' && str[i + 1] <= 'z'))) 
                return 0;
        }  
    }
    if(!flag_at)    return 0;
    return 1;
}

int userConfirmation(int option)//USER INPUT VALIDATION FOR USER'S OPTION
{
    scanf("%d",&option);
    if(option ^ 1 && option ^ 0)  option = invalid_input(option);

    return option;
}
/****************************************************************************************************************************/
/**************************************************EDIT OPTION FUNCTION DEFINTIONS****************************************************/
int editingPage(int option, int function_no)
{
    //TABLE
    printf("\n%25s"," ");
    print_line();
    printf("\n%41s%49s\n%41s%19s\033[1;37;45m %s \033[0m%19s\n%41s%49s\n%25s", "|", "|", "|", " ", " CONTACT ", "|", "|", "|", " ");
    print_line();

    printf("\n%41s%49s\n%41s%7s%21s%21s\n%41s%49s\n%41s%11s%25s%13s\n%41s%49s\n%41s%9s%20s%20s\n%41s%49s\n%41s%12s%11s%26s\n%41s%49s\n%25s", "|", "|", "|", " ", "1.EDIT NAME", "|", "|", "|", "|", " ", "2.EDIT PHONE NUMBER", "|", "|", "|", "|", " ", "3.EDIT EMAIL", "|", "|", "|", "|", " ", "4.EXIT", "|", "|", "|", " ");
    print_line();
    printf("\n");

    //ASKING USER TO ENTER AN OPTION
    printf("\n\n%15s\033[1mPlease enter an option : \033[0m", " ");
    start:
    scanf("%d",&option);

    switch(option)
    {
            case 1: return 1;

            case 2: return 2;
        
            case 3: return 3;

            case 4: return 0;

            default:
                printf("\n%15s\033[1;37;41m:::!!!INVALID INPUT!!!:::\033[0m\n\n%15s\033[1mPlease enter a correct option\033[0m (Ex : 2) : ", " ", " ");
                goto start;
    }
}

void editing_contactDetails(AddressBook *addressBook,int serial_no, int option, FILE *fp)
{
    short res = 0;
    while(loopRunner)
    {
        //INITIALIZING STRINGS
        char *new_name = malloc(50 * sizeof(char));
        char *new_phone_number = malloc(20 * sizeof(char));
        char *new_email = malloc(50 * sizeof(char));

        int invalid_count = 0;
        int opt = editingPage(0, option);
        if(!opt)    
        {
            loading(3);
            break;
        }

        if(!(opt ^ 1)) //EDIT NAME
        {  
            start_1:
            printf("\n%15sEnter Name to EDIT a contact : ", " ");
            scanf(" %[^\n]",new_name);

            exitFlag =  name_validate(new_name);
            if(!exitFlag)
            {   
                print_invalid_name();
                res = re_entry_print(&invalid_count);
                if(res) goto start_1;
                else continue;
            }
            //ASK USER CONFIRMATION
            printf("\n\n%15s\033[1;37;43m!ARE YOU SURE YOU WANT TO EDIT '%s' AS CONTACT NAME!\033[0m (1 - Yes) / (0 - No) : ", " ", new_name);
            int opt = userConfirmation(0);
            printf("\n\n");
            print_line();
            if(!opt)    
            {
                loading(3);
                break;
            }
            // copy with to addressBook->contacts[serial_no].name
            strcpy(addressBook->contacts[serial_no].name, new_name); 
            
            printf("\n%15s\033[1;42m:::CONTACT EDITED:::\033[0m\n", " ");
            print_line(); 
        }
        else if(!(opt ^ 2)) //EDIT PHONE NUMBER
        { 
            start:
            getchar();
            printf("\n%15sEnter Phone Number to EDIT a contact : ", " ");
            scanf("%[^\n]",new_phone_number);
            
            exitFlag =  phone_number_validate(new_phone_number);

            //CHECK WITH REMAINING PHONE NUMBERS
            if(exitFlag)
            {
                int i = 0;
                while(i < addressBook->contactCount)
                {
                    if(!(strcmp(new_phone_number, addressBook->contacts[i].phone)))
                    {
                        printf("\n%15s\033[1;37;41m:::!!!Entered Phone Number already exist in another contact!!!:::\033[0m\n\n%15s\033[1mTry again :-)\033[0m \n"," ", " ");
                        goto re_enter_limit_1;
                    }
                    i++;
                }
            }
            if(!exitFlag)
            {   
                print_invalid_phone_number();
                   re_enter_limit_1:
                res = re_entry_print(&invalid_count);
                if(res) goto start;
                else continue;
            }
            //ASK USER CONFIRMATION
            printf("\n\n%15s\033[1;37;43m!ARE YOU SURE YOU WANT TO EDIT '%s' AS CONTACT PHONE NUMBER!\033[0m (1 - Yes) / (0 - No) : ", " ", new_phone_number);
            int opt = userConfirmation(0);
            printf("\n\n");
            print_line();
            if(!opt)    
            {
                loading(3);
                break;
            }
            // copy with addressBook->contacts[serial_no].phone
            strcpy(addressBook->contacts[serial_no].phone, new_phone_number);     
            printf("\n%15s\033[1;42m:::CONTACT EDITED:::\033[0m\n", " ");
        }
        else if(!(opt ^ 3))   //EDIT EMAIL
        {
            start_2:
            getchar();
            printf("\n%15sEnter Email ID to EDIT a contact : ", " ");
            scanf("%[^\n]",new_email);

            exitFlag =  email_validate(new_email);
            //CHECK WITH REMAINING EMAIL IDS
            if(exitFlag)
            {
                int i = 0;
                while(i < addressBook->contactCount)
                {
                    if(!(strcmp(new_email, addressBook->contacts[i].email)))
                    {
                        printf("\n%15s\033[1;37;41m:::!!!Entered Email ID already exist in another contact!!!:::\033[0m\n\n%15s\033[1mTry again :-)\033[0m \n", " ", " ");
                        goto re_enter_limit_2;
                    }
                    i++;
                }  
            }
            if(!exitFlag)
            {   
                print_invalid_email(); 
                    re_enter_limit_2:
                res = re_entry_print(&invalid_count);
                if(res) goto start_2;
                else continue;
            }
            //ASK USER CONFIRMATION
            printf("\n\n%15s\033[1;37;43m!ARE YOU SURE YOU WANT TO EDIT '%s' AS CONTACT EMAIL ID!\033[0m (1 - Yes) / (0 - No) : ", " ", new_email);
            int opt = userConfirmation(0);
            printf("\n\n");
            print_line();
            if(!opt)    
            {
                loading(3);
                break;
            }
            // copy with addressBook->contacts[serial_no].email
            strcpy(addressBook->contacts[serial_no].email, new_email); 
            printf("\n%15s\033[1;42m:::CONTACT EDITED:::\033[0m\n", " ");
            print_line();
        }
        fp = fopen("AddressBook.csv","w");
        rewind(fp);
        fprintf(fp,"#%d#\n",(addressBook->contactCount));

        for(int i = 0; i < addressBook->contactCount; i++)
            fprintf(fp,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);

        fclose(fp);
        free_up_str(new_name,new_phone_number);
        free(new_email);
    }   
}
/******************************************************************************************************/
