//strlib.c
#include <stdlib.h>


int mystrlen(char *str) {
    int len = 0;
    for(int i = 0; str[i] != 0; i++){
        len++;
    }
    return len;
}

char * mystradd(char *first, char *last) {
    int len = mystrlen(first) + mystrlen(last);
    char *final = malloc(sizeof(char)*len);
    for(int i = 0; i < mystrlen(first); i++){
        final[i] = first[i];
    }
    int count = 0;
    for(int i = mystrlen(first); i < len; i++){
        final[i] = last[count];
        count++;
    }
    return final;
}

int mystrfind(char *complete, char *substr){
    int lenComplete = mystrlen(complete);
    int lenSub = mystrlen(substr);

    for (int i = 0; i < lenComplete-lenSub+1; i++){
        for(int j = 0; j < lenSub; j++) {
            if(substr[j] == complete[i+j]){
                return 1;
            }
        }
    }
    return 0;
}
