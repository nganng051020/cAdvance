#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"clear.h"
#include"inc/btree.h"

int main(){
    int menu;
    int size;
    char name[80];
    long number;
    BTA* book;
    btinit();
    book = btopn("mybook",0,0);
    if(!book)   {
        book = btcrt("mybook",0,0);
        number = 435465;
        if(btins(book, "Nguyen Ngan", (char*)&number, sizeof(long)) != 0)
        printf("ERROR\n");
        number = 3984654;
        if(btins(book, "Minh Hieu", (char*)&number, sizeof(long)) != 0)
        printf("ERROR\n");
        if(btins(book, "Cam Li", (char*)&number, sizeof(long)) != 0)
        printf("ERROR\n");}

    do{
        printf("\n--------------PHONEBOOKbt-------------ntn\n");
        printf("1. Print list\n");
        printf("2. Delete\n");
        printf("3. Get number\n");
        printf("4. Add number\n");
        printf("5. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            btsel(book, "", (char*)&number, sizeof(long), &size);
            while(btseln(book, name, (char*)&number, sizeof(long), &size) == 0)
              {  printf("%-25s%-20ld\n", name, number);}
            break;
        case 2:
            clean_stdin();
            printf("Name: ");
            gets(name);
            if(btdel(book, name) == 0) printf("Delete successful\n");
            break;
        case 3:
            clean_stdin();
            printf("Name: ");
            gets(name);
            if(btsel(book, name, (char*)&number, sizeof(long), &size) == 0)
               printf("Phone of %s is %ld\n", name, number);
            else
               printf("Not found!\n");
            break;
        case 4:
            clean_stdin();
            printf("Name: ");
            gets(name);
            printf("Phone number: ");
            scanf("%ld", &number);
            if(btins(book, name, (char*)number, sizeof(long)) != 0)
               printf("ERROR\n");
            break;
            
        default:
            btcls(book);
            printf("\nThank for using the program-ntn-\n\n");
            break;
        }


    }while(menu != 5);

    return 0;

}