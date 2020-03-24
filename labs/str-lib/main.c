//main.c
#include <stdlib.h>
#include <stdio.h>

int mystrlen(char*);
char * mystradd(char*,char*);
int mystrfind(char*,char*);


int main(int argc, char **argv){
    printf("Initial Lenght      : %d\n",mystrlen(argv[1]));
    char *newStr = mystradd(argv[1], argv[2]);
    printf("New String          : %s\n",newStr);
    int result = mystrfind(newStr, argv[3]);
    char *yesno = "yes";
    if(result == 0){
        yesno = "no";
    }
    printf("SubString was found : %s\n",yesno);
    return 0;
}

