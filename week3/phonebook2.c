#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"clear.h"
#define INITIAL_SIZE 10
#define INCREMENTAL_SIZE 5

typedef struct {
	void * key;
	void * value;
} Entry;

typedef struct {
	Entry * entries;
	int size, total;
	Entry (*makeNode)(void*, void*);
	int (*compare)(void*, void*);
} SymbolTable;

SymbolTable createSymbolTable(
		Entry (*makeNode)(void*, void*),
		int (*compare)(void*, void*)
);

void dropSymbolTable(SymbolTable* tab);
void addEntry(void* key, void* value, SymbolTable* book);
Entry* getEntry(void* key, SymbolTable book);

Entry makePhone(void* name, void* phone) {
	Entry res;
	res.key = strdup( (char*)name );
	res.value = malloc(sizeof(long));
	memcpy( res.value, phone, sizeof(long) );
	return res;
}
int comparePhone(void * key1, void* key2) {
	return strcmp((char*)key1, (char*)key2);
}

int main()
{
    int menu;
    long number;
    char name[80];
    Entry *entry;
    SymbolTable book = createSymbolTable(makePhone, comparePhone);
    addEntry("Nguyen Ngan", 985748, &book);
    addEntry("Cam Li", 4567676, &book);
    addEntry("Minh Hieu", 978465, &book);
    addEntry("Chi Thuan", 9475864, &book);
    
    do{
        printf("\n----------PHONEBOOK----------ntn\n");
        printf("1. Add phone\n");
        printf("2. Search phone\n");
        printf("3. Print list\n");
        printf("4. Exit\n");
        printf("->Your choice: ");
        scanf("%d",&menu);
        switch(menu){
              case 1:
                   clean_stdin();
                   printf("Name:");
                   gets(name);
                   printf("Number:");
                   scanf("%ld",&number);
                   addEntry(name, &number, &book);
                   break;
              case 2:
                   clean_stdin();
                   printf("\nName:");
                   gets(name);
                   entry = getEntry(name, book);
                   if (entry==NULL) printf("Not found!");
                   else printf("Phone number of %s is %ld", name, *((long*)entry->value));
                   break;
              case 3:
                   printf("\n%-20s%-15s\n", "Name", "Phone Number");
                   for (int i = 0; i < book.total; i++)
                       printf("%-20s%-15ld\n", book.entries[i].key, *((long*)book.entries[i].value));
                   break;
              default:         
                   dropSymbolTable(&book);
                   printf("\nThank for using the program!-ntn-\n\n");
                   break;
          }
     }while(menu != 4);

     return 0;
}

SymbolTable createSymbolTable(Entry (*makeNode)(void*, void*), int (*compare)(void*, void*) )
{
    SymbolTable p;
    p.total = 0;
    p.size = INITIAL_SIZE;
    p.entries = (Entry*)malloc(INITIAL_SIZE*sizeof(Entry));
    p.makeNode = makeNode;
    p.compare = compare;
    return p;
}

void dropSymbolTable(SymbolTable* tab)
{
    for (int i = 0; i < tab->total; i++)
    {
        free(tab->entries[i].key);
        free(tab->entries[i].value);
    }
    free(tab->entries);
    tab->total = 0;
    tab->size = 0;
}

int binarySearch(Entry* entries, int l, int r, void* key, int* found, int (*compare)(void*, void*))
{
	int i, res;
    if (r < l)
    {
       *found = 0;
       return l;
    }   
	i = (l + r)/2;
	res = compare(key, entries[i].key);
	if (res==0)
	{
        *found = 1;       
    	return i;
    }else if (res < 0)
		return binarySearch(entries, l, i-1, key, found, compare);
    else
        return binarySearch(entries, i+1, r, key, found, compare);
     
}

void addEntry(void* key, void* value, SymbolTable* book)
{
    int found, pos;
    if (book->total > 0)
    {
        pos = binarySearch(book->entries, 0, book->total-1, key, &found, book->compare);
    }
    else
    {
        pos = 0;
        found = 0;     
    }   
    if (found)
    {
        free(book->entries[pos].key);
        free(book->entries[pos].value);
        book->entries[pos] = book->makeNode(key, value);
    }
    else 
    {
        if(book->total >= book->size) 
        {
            Entry *p;
            int newsize = (book->size+INCREMENTAL_SIZE);
            p = (Entry*) malloc(newsize*sizeof(Entry));
            memcpy(p, book->entries, book->total*sizeof(Entry));
            free(book->entries); 
            book->entries = p;
            book->size = newsize;
        }
        if (pos < book->total) 
        {
            memcpy(&(book->entries[pos+1]), &(book->entries[pos]), (book->total-pos)*sizeof(Entry));
        }
        book->entries[pos] = book->makeNode(key, value);
        book->total++;       
    } 
}

Entry* getEntry(void* key, SymbolTable book)
{
     int found, pos;
     pos = binarySearch(book.entries, 0, book.total-1, key, &found, book.compare);
     if (found)
        return &(book.entries[pos]);
     else
        return NULL;
}
