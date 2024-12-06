#ifndef CONTACT_H
#define CONTACT_H


#include <stdio.h>
#include <string.h>

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact *contacts;
    int contactCount;
} AddressBook;

// Declare the structure containing the same members and this strucure is used for copying the modified contacts to the file
struct SAMPLE_CONTACTS {
    char sample_name[100];
    char sample_phone[20];
    char sample_email[100];
};

void createContact(AddressBook *addressBook, FILE *fp);
void searchContact(AddressBook *addressBook, FILE *fp);
void editContact(AddressBook *addressBook, FILE *fp);
void deleteContact(AddressBook *addressBook, FILE *fp);
void listContacts(AddressBook *addressBook, FILE *fp);
void initialize(AddressBook *addressBook);
void loadContact(AddressBook *addressBook, FILE *fp);//create another array of struct and copy the contacts from .csv //read data != ','
void saveContact(struct SAMPLE_CONTACTS *samplecontacts, FILE *fp, int countContact);


void print_line();

void loading(int usage);

int invalid_input(int iter);

void free_up_str(char *str1, char *str2);

int userConfirmation(int option);

#endif
