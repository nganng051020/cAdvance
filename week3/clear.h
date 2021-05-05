void clean_stdin(void){
    char c;
    do{
        c = getchar();
    }while(c != '\n' && c != EOF);
}