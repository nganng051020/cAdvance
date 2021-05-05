#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"clear.h"
#define INTIAL_SIZE 10
#define INCREMENTAL_SIZE 5

typedef struct{
    char name[80];
    long number;
}PhoneEntry;

typedef struct{
    PhoneEntry *entries;
    int total;
    int size;
}PhoneBook;

PhoneBook createPhoneBook();
void dropPhoneBook(PhoneBook* book);
int binarySearch(PhoneEntry* entries, int l, int r, char *name, int* found);
void addPhoneNumber(char* name, long number, PhoneBook *book);
PhoneEntry *getPhoneNumber(char* name, PhoneBook book);

int main(){
    int menu;
    char name[80];
    long number;
    PhoneEntry *entry;
    PhoneBook book;
    book = createPhoneBook();
    addPhoneNumber("Nguyen Ngan", 345678, &book);
    addPhoneNumber("Cam Li", 867575, &book);
    addPhoneNumber("Minh Hieu", 6755673, &book);

    do{
        printf("\n---------PHONEBOOK----------ntn\n");
        printf(" 1. Add phone\n");
        printf(" 2. Search phone\n");
        printf(" 3. Print list\n");
        printf(" 4. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            printf("\nNumber: ");
            scanf("%ld", &number);
            clean_stdin();
            printf("Name: ");
            gets(name);
           
            addPhoneNumber(name, number, &book);
            break;
        case 2:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            entry = getPhoneNumber(name, book);
            if(entry == NULL) printf("Not found!!!\n");
            else printf("Phone number of %s is %ld\n", name, entry->number);
            break;
        case 3:
            printf("\n");
            printf("%-20s%-15s\n", "Name", "Phone_Number");
            for(int i = 0; i < book.total; i++)
              printf("%-20s%-15ld\n", book.entries[i].name, book.entries[i].number);
            break;
        default:
            dropPhoneBook(&book);
            printf("\nThank for using this program!!!-ntn-\n\n");
            break;
        }
    }while(menu!=4);

    return 0;
}

PhoneBook createPhoneBook(){
    PhoneBook newBook;
    newBook.total = 0;
    newBook.size = INTIAL_SIZE;
    newBook.entries = (PhoneEntry*)malloc(INTIAL_SIZE*sizeof(PhoneEntry));
    return newBook;
}

void dropPhoneBook(PhoneBook* book){
    book->total = 0;
    book->size = 0;
    free(book->entries);
}

int binarySearch(PhoneEntry* entries, int l, int r, char *name, int *found){
    int i, res;
    if(r<l){
        *found = 0;
        return l;
    }
    i = (r+l)/2;
    res = strcmp(name, entries[i].name);
    if(res==0){
        *found = 1;
        return i;
    }
    else if(res<0) return binarySearch(entries,l,i-1,name,found);
    else return binarySearch(entries,i+1,r,name,found);
}

void addPhoneNumber(char* name, long number, PhoneBook* book){
    int found, pos;
    if(book->total > 0){
        pos = binarySearch(book->entries,0,book->total-1, name, &found);
    }
    else{
        pos = 0;
        found = 0;
    }
    if(found){
        book->entries[pos].number = number;
    }
    else{
        if(book->total >= book->size){
            PhoneEntry *p;
            int newsize = (book->size + INCREMENTAL_SIZE);
            p = (PhoneEntry*)malloc(newsize*sizeof(PhoneEntry));
            memcpy(p,book->entries,book->total*sizeof(PhoneEntry));
            free(book->entries);
            book->entries = p;
            book->size = newsize;
        }
        if(pos < book->total){
            memcpy(&(book->entries[pos+1]), &(book->entries[pos]), (book->total-pos)*sizeof(PhoneEntry));
        }

        strcpy(book->entries[pos].name, name);
        book->entries[pos].number = number;
        book->total++;
    }
}

PhoneEntry *getPhoneNumber(char* name, PhoneBook book){
    int found, pos;
    pos = binarySearch(book.entries, 0, book.total-1, name, &found);
    if(found) return &(book.entries[pos]);
    else return NULL;
}