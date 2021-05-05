#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"clear.h"
#define INTIAL_SIZE 10
#define INCREMENTAL_SIZE 5

typedef struct{
    char name[80];
    float grade;
}Student;

typedef struct{
    Student* entries;
    int total;
    int size;
}listStudent;

listStudent createList();
void dropList(listStudent *std);
void addGrade(char *name, float grade, listStudent *std);
void sortGrade(listStudent* std);
Student *getGrade(char* name, listStudent std);
Student *getName(float grade, listStudent std);
int delete(char* name, listStudent *std);

int main(){
    int x;
    int menu;
    char name[80];
    float grade;
    Student *entry;
    listStudent std;
    std = createList();

    addGrade("Nguyen Ngan", 7.8, &std);
    addGrade("Cam Li", 8.5, &std);
    addGrade("Minh Hieu", 8.0, &std);
    addGrade("Chi Thuan", 7.5, &std);

    do{
        printf("\n---------Quan li sinh vien---------ntn\n");
        printf("1. Print list\n");
        printf("2. Search grade\n");
        printf("3. Search name\n");
        printf("4. Sorted by grade\n");
        printf("5. Delete student\n");
        printf("6. Add student\n");
        printf("7. Exit\n");
        printf("->Your choice: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            printf("\n\n%-20s%-15s\n", "Name", "Grade");
            for(int i = 0; i < std.total; i++){
              printf("%-20s%-15.2f\n", std.entries[i].name, std.entries[i].grade);
            }
            break;
        case 2:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            entry = getGrade(name, std);
            if(entry==NULL) printf("Not found!\n");
            else printf("Grade of %s is %.2f\n", name, entry->grade);
            break;
        case 3:
            printf("\nGrade: ");
            scanf("%f", &grade);
            entry = getName(grade, std);
            if(entry==NULL) printf("Not found!\n");
            else printf("Student had %.2f mark is %s\n", grade, entry->name);
            break;
        case 4:
            sortGrade(&std);
            printf("\n\n%-20s%-15s\n", "Name", "Grade");
            for(int i = 0; i < std.total; i++)
              printf("%-20s%-15.2f\n", std.entries[i].name, std.entries[i].grade);
            break;
        case 5:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            x = delete(name, &std);
            if(x == 0) printf("Not found!\n");
            else printf("Delete successful!!!\n");
            break;
        case 6:
            clean_stdin();
            printf("\nName: ");
            gets(name);
            printf("Grade: ");
            scanf("%f", &grade);
            addGrade(name,grade,&std);
            break;
        default:
            dropList(&std);
            printf("\nThank for using the program!!!--ntn--\n\n");
            break;
        }
    }while(menu!=7);

    return 0;
}

listStudent createList(){
    listStudent newlist;
    newlist.total = 0;
    newlist.size = INTIAL_SIZE;
    newlist.entries = (Student*)malloc(INTIAL_SIZE*sizeof(Student));
    return newlist;
}

void dropList(listStudent* std){
    free(std->entries);
    std->size = 0;
    std->total = 0;
}

int binarySearch(Student* entries, int l, int r, char* name, int *found){
    int i, res;
    if(r<l){
        *found = 0;
        return l;
    }
    i = (l+r)/2;
    res = strcmp(name, entries[i].name);
    if(res == 0){
        *found = 1;
        return i;
    }
    else if(res < 0) return binarySearch(entries, l, i -1, name, found);
    else return binarySearch(entries, i+1, r, name, found);
}

int binarySearchGrade(Student *entries, int l, int r, float grade, int *found){
    int i;
    if(r<l){
        *found = 0;
        return l;
    }
    i = (l+r)/2;
    if(grade == entries[i].grade){
        *found = 1;
        return i;
    }
    else if(grade < entries[i].grade) return binarySearchGrade(entries,l,i-1,grade,found);
    else return binarySearchGrade(entries,i+1,r,grade,found);
}

void addGrade(char* name, float grade, listStudent* std){
    int found, pos;
    if(std->total > 0){
        pos = binarySearch(std->entries, 0, std->total - 1, name, &found);
    }
    else{
        pos = 0;
        found = 0;
    }
    if(found){
        std->entries[pos].grade = grade;
    }
    else{
        if(std->total >= std->size){
            Student* p;
            int newsize = (std->size +INCREMENTAL_SIZE);
            p = (Student*)malloc(newsize*sizeof(Student));
            memcpy(p, std->entries, std->total*sizeof(Student));
            free(std->entries);
            std->entries = p;
            std->size = newsize;
        }
        if(pos < std->total){
            memcpy(&(std->entries[pos+1]),&(std->entries[pos]), (std->total-pos)*sizeof(Student));
        }
        strcpy(std->entries[pos].name, name);
        std->entries[pos].grade = grade;
        std->total++;
    }
}

Student *getGrade(char *name, listStudent std){
    int found, pos;
    pos = binarySearch(std.entries, 0, std.total-1, name, &found);
    if(found)
       return &(std.entries[pos]);
    else
       return NULL;   
}

void sortGrade(listStudent* std){
    for(int i = 0; i < std->total; i++)
        for(int j = 0; j < std->total-i-1; j++){
            if(std->entries[j].grade < std->entries[j+1].grade){
                Student p;
                p = std->entries[j];
                std->entries[j] = std->entries[j+1];
                std->entries[j+1] = p;
            }
        }
}

Student *getName(float grade, listStudent std){
    int found, pos;
    pos = binarySearchGrade(std.entries, 0, std.total-1,grade, &found);
    if(found)
        return &(std.entries[pos]);
    else
        return NULL;   
}

int delete(char* name, listStudent *std){
    int pos, found;
    pos = binarySearch(std->entries, 0, std->total-1, name, &found);
    if(found){
        for(int i = pos; i < std->total; i++)
            std->entries[i] = std->entries[i+1];
        std->total--;
        return 1;
    }
    else
        return 0;
    
}