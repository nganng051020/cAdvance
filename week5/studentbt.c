#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"clear.h"
#include"inc/btree.h"

int main(){
    int menu, size;
    char name[80];
    float grade, gradeSearch;
    BTA* student;
    btinit();
    student = btopn("std", 0, 0);
    if(!student){
        student = btcrt("std", 0, 0);
        grade = 8.2;
        if(btins(student, "Nguyen Ngan", (char*)&grade, sizeof(float)) != 0) printf("ERROR\n");
        grade = 8.0;
        if(btins(student, "Cam Li", (char*)&grade, sizeof(float)) != 0) printf("ERROR\n");
        grade = 8.3;
        if(btins(student, "Minh Hieu", (char*)&grade, sizeof(float)) != 0) printf("ERROR\n");
    }

    do{
        printf("\n----------------STUDENT_MANAGEMENT------------ntn\n");
        printf("1. Print list\n");
        printf("2. Delete student\n");
        printf("3. Search name\n");
        printf("4. Search grade\n");
        printf("5. Sort grade\n");
        printf("6. Add student\n");
        printf("7. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu){
        case 1:
              printf("\n%-25s%-10s\n", "Name", "Grade");
              btsel(student, "", (char*)&grade, sizeof(float), &size);
              while(btseln(student, name, (char*)&grade, sizeof(float), &size) == 0)
              printf("%-25s%-10.2f\n", name, grade);
              break;
        case 2:
              clean_stdin();
              printf("Name: ");
              gets(name);
              if(btdel(student, name) == 0) printf("Delete successful\n");
              else printf("ERROR\n");
              break;
        case 3:
              clean_stdin();
              printf("Name: ");
              gets(name);
              if(btsel(student,name,(char*)&grade, sizeof(float), &size) == 0)
                  printf("Grade of %s is %.2f\n", name, grade);
              else
                  printf("Not found!\n");
              break; 
        case 4:
              printf("Grade: ");
              scanf("%f", &gradeSearch);

              btsel(student,"", (char*)&grade, sizeof(float), &size);
              while(btseln(student, name, (char*)&grade, sizeof(float), &size) == 0){
                  if(gradeSearch == grade)
                  printf("%-25s%-10.2f\n", name, grade);
              }
              break;
        case 5:
        
              
        case 6:
              clean_stdin();
              printf("Name: ");
              gets(name);
              printf("Grade: ");
              scanf("%f", &grade);
              if(btins(student, name, (char*)&grade, sizeof(float)) != 0)
                 printf("ERROR\n");
              else printf("Add student successful\n");
            break;
        default:
            btcls(student);
            break;
        }
    }while(menu != 7);

    return 0;
}