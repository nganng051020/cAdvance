#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"clear.h"
#include"jrb.h"
#include"jval.h"

int main(){
    int menu;
    char name[80];
    long number;
    JRB node;
    JRB book = make_jrb();

    jrb_insert_str(book, strdup("Cam Li"), new_jval_l(4545465));
    jrb_insert_str(book, strdup("Nguyen Ngan"), new_jval_l(4354353));


    do{
        printf("----------PHONEBOOKjrb--------ntn\n");
        printf("1. Add phone number\n");
        printf("2. Search phone number\n");
        printf("3. Print list\n");
        printf("4. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
               clean_stdin();
               printf("\nName: ");
               gets(name);
               printf("Phone number: ");
               scanf("%ld", &number);
               node = jrb_find_str(book, name);
               if(node != NULL )
                   node->val = new_jval_l(number);
               else
                   jrb_insert_str(book, strdup(name), new_jval_l(number));
               break;
        case 2:
               clean_stdin();
               printf("\nName: ");
               gets(name);
               node = jrb_find_str(book, name);
               if(node == NULL) printf("Not found!\n");
               else printf("Phone number of %s is %ld\n", name, jval_l(node->val));
               break;
        case 3:
               jrb_traverse(node, book)
                   printf("%-20s%-15ld\n", jval_s(node->key), jval_l(node->val));
               break;
        default:
               jrb_traverse(node,book)
                   free(jval_s(node->key));
               jrb_free_tree(book);
               printf("\nThank for using the program!-ntn-\n");
               break;
        }
    }while(menu!=4);
    return 0;

}

