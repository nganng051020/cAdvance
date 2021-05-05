#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"btree.h"
#include"clear.h"

int main(){
    char name[80];
    long number;
    int *size;
    int menu;
    BTA* book;
    btinit();
    book = btopn("mybook",0,0);
    if(!book) book=btcrt("mybook",0,0);

    do{
        printf("\n-----------PHONEBOOKbt----------ntn\n");
        printf("1. Print list\n");
        printf("2. Add number\n");
        printf("3. Search number\n");
        printf("4. Delete\n");
        printf("5. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            Printf("\n-25%s-20%s\n", "Name","Phone number");
            btsel(book,"",(char*)&number, sizeof(long), &(*size));
            while(btsel(book, name, (char*)&number, sizeof(long), &(*size))==0)
                printf("%-25s%-20ld\n", name, number);
            break;
        case 2:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            printf("Phone number: ");
            scanf("%ld", &number);
            while(getchar()!='\n');
            btins(book, name, (char*)&number, sizeof(long));
            break;
        case 3:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            if(btsel(book, name, (char*)&number, sizeof(long), &(*size))==0)
               printf("Phone of %s is %ld\n\n", name, number);
            else
               printf("Not found!\n\n");
            break;
        case 4:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            if(btdel(book, name) == 0)
              printf("Delete Successful!\n");
            break;
        default:
            btcls(book);
            printf("\nThank for using the program!-ntn-\n\n");
            break;
        }
    }while(menu != 5);

    return 0;
}
