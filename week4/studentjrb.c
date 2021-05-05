#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"clear.h"
#include"jrb.h"
#include"jval.h"

int main(){
    int menu;
    int i = 0;
    char name[80];
    char *a = (char*)malloc(100*sizeof(char));
    float grade;
    JRB node, p;
    JRB student = make_jrb();

    jrb_insert_str(student, strdup("Nguyen Ngan"), new_jval_f(8.2));
    jrb_insert_str(student, strdup("Cam Li"), new_jval_f(8.5));
    jrb_insert_str(student, strdup("Minh Hieu"), new_jval_f(8.6));

    do{
        printf("-----------Student Management----------ntn\n");
        printf("1. Print list and average grade\n");
        printf("2. Add student\n");
        printf("3. Delete student\n");
        printf("4. Sort by grade\n");
        printf("5. Search grade\n");
        printf("6. Search name\n");
        printf("7. Free and Exit!\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
               printf("\n%-20s%-10s\n", "Name", "Grade");
               jrb_traverse(node, student){
                 printf("%-20s%-10.2f\n", jval_s(node->key), jval_f(node->val));
                }
               break;
        case 2:
               clean_stdin();
               printf("\nName: ");
               gets(name);
               printf("Grade: ");
               scanf("%f", &grade);
               node = jrb_find_str(student, name);
               if(node != NULL)
                   node->val = new_jval_f(grade);
               else
                   jrb_insert_str(student, strdup(name), new_jval_f(grade));
               break;
        case 3:
               clean_stdin();
               printf("\nName: ");
               gets(name);
               node = jrb_find_str(student, name);
               if(node == NULL) printf("Not found %s\n\n", name);
               else {jrb_delete_node(node);
                     printf("Delete successful!\n\n");}
               break;
        case 4:
               p = make_jrb();
               jrb_traverse(node, student){
                   a = strdup(jval_s(node->key));
                   jrb_insert_dbl(p, jval_f(node->val), new_jval_s(a));
               }
               jrb_traverse(node,p)
                   printf("%-20s%-10.2f\n", jval_s(node->val), jval_d(node->key));
               printf("\n");
               break;
        case 5:
              clean_stdin();
              printf("\nName: ");
              gets(name);
              node = jrb_find_str(student, name);
              if(node == NULL) printf("Not found!!!\n");
              else printf("Grade of %s is %.2f\n\n", jval_s(node->key), jval_f(node->val));
              break;
        case 6:
              printf("Grade: ");
              scanf("%f", &grade);
              jrb_traverse(node, student){
                  if(grade == jval_f(node->val))
                    { printf("%-20s%-10.2f\n", jval_s(node->key), jval_f(node->val));}
              }
              printf("\n");
              break;
        default:
            jrb_traverse(node, student) free(jval_s(node->key));
            jrb_free_tree(student);
            printf("\nThank for using the program!-ntn-\n\n");
            break;
        }
    }while(menu != 7);

    return 0;
}