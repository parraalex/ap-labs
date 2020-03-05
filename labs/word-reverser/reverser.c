
#include <stdio.h>

void reverse(int lenght, char arr[]) {

    int i;
    char tmp;

    for (i = 0;  i < lenght/2; i++) {
        tmp = arr[i];
        arr[i] = arr[lenght - i - 1];
        arr[lenght - i - 1] = tmp;
    }
}


int main(){
int i = 0;
char name[20]; 

printf("\nEnter the Name : "); 

while((name[i] = getchar())!='\n')
        i++ ;

reverse(i,name);
printf("%s",name);
return 0;
}
